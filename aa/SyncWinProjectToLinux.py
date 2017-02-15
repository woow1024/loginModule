#!/usr/bin/env python
#-*- coding: gb2312 -*-

import os
import sys

import paramiko

Config = { "Server"             : "192.168.44.223",
           "Username"           : "shanheng.wu",
           "Passwd"             : "pcbest",
           "ProjectRoot"        : r"F:\GWStation4\QCenter",                                  #本地项目目录
           "RemoteRoot"         : "/home/shanheng.wu/workspace",		                               #远程LINUX项目目录
           "ProjectName"        : "QCenter",                                                 #项目名称
           "SyncFileExt"        : ".cpp,.h,.txt,.c,.xml",
           "CMakeListFileName"  : "CMakeLists.txt",
           "ShellCMD"           : "cd /home/shanheng.wu/workspace",                              #上传完成后执行的命令,暂时不使用
           "AskBeforSync"       : False,
           }

g_localfile_map = {}
g_dir_add_queue = []
g_operation_queue = []
g_rebuild_makefile = False

def get_linux_path(rel_path, filename = ""):
    file_path = Config["RemoteRoot"] + rel_path.replace("\\", "/")

    if filename == "":
		file_path = "%s%s" % (Config["RemoteRoot"], rel_path.replace("\\", "/"))
    else:
		file_path = "%s%s/%s" % (Config["RemoteRoot"], rel_path.replace("\\", "/"), filename)

    return file_path

def excute_ssh_commad(ssh, cmd):
    #print cmd
    _, stdout, stderr = ssh.exec_command(cmd)

    ssh_msg = stdout.readlines()

    for msg in ssh_msg:
		print msg,      

    ssh_msg = stderr.readlines()

    for msg in ssh_msg:
		print msg,       

def proceed_operation(op, ssh, sftp):
#operation format [(0)filetype, (1)file_hash, (2)lastmodifytime, (3)filename, (4)localpath, (5)relativepath, (6)optype]
    remote_path = get_linux_path(op[5], op[3])

    if op[0] == "D":    #目录
		shell_cmd = "" 

		if op[6] == "D":  
			print "delete dir", remote_path
			shell_cmd = "rm -r "
		else:
			print "create dir", remote_path
			shell_cmd = "mkdir -p "
	
		shell_cmd += remote_path
	
		excute_ssh_commad(ssh, shell_cmd)       
    else:
		if op[6] == "N":          
			print "new :", remote_path
			sftp.put(op[4], remote_path)
		elif op[6] == "M":
			print "update :", remote_path
			sftp.put(op[4], remote_path)
		else:
			print "delte :", remote_path
			try:
				sftp.remove(remote_path) 
			except IOError, e:
				if e.errno != 2:
					print e.errno, e.message

def file_synv():
    global g_rebuild_makefile

    if not g_operation_queue:
		return True

    if Config["AskBeforSync"]:
		answer = raw_input("begin file sync?[y/n]")

		if answer == "n" or answer == "N":
			return False

    try:
		ssh = paramiko.SSHClient()
		ssh.set_missing_host_key_policy(paramiko.AutoAddPolicy())
		ssh.connect(Config["Server"] ,  username = Config["Username"],  password = Config["Passwd"])  
		sftp = ssh.open_sftp()
    except Exception, e:
		print e.message
		return False

    for op in g_operation_queue:
		proceed_operation(op, ssh, sftp)        

    shell_cmd = "cd %s/%s" % (Config["RemoteRoot"], Config["ProjectName"])

    if g_rebuild_makefile:
		shell_cmd += ";cmake .; make clean; make"
    else:
		shell_cmd += ";make"

    print shell_cmd    
    excute_ssh_commad(ssh, shell_cmd)

    return True

def load_local_env(project_name):
    metefile_path =  "%s.mete" % os.path.join(os.path.split(sys.argv[0])[0], project_name)

    try:
		f = open(metefile_path)

		for line in f.readlines():
			if not line:
				continue
	
			line = line.strip("\n")
			info_array = line.split(";")
			lens = len(info_array)
			if len(info_array) != 6:
				print "meta file data format error!", line
				continue
	
			file_key = int(info_array[1])
			g_localfile_map[file_key] = [info_array[0], file_key, int(info_array[2]), info_array[3], info_array[4], info_array[5]]

		f.close()

		return True
    except IOError, e:
		if e.errno == 2:
			return True

		print e.message
		return False
    except Exception, e:
		print e.message
		return False

def check_if_need_create_dir(dir_path, dir_rel_path):
    dir_key = dir_path.__hash__()

    if g_localfile_map.has_key(dir_key):
		return

    g_localfile_map[dir_key] = ["D", dir_key, 0, "", dir_path, dir_rel_path]  

    if g_dir_add_queue.count(dir_key) == 0:
		g_dir_add_queue.append(dir_key)
		g_operation_queue.append(["D", dir_key, 0, "", dir_path, dir_rel_path, "N"])

def project_file_check(project_name):
	global g_rebuild_makefile
	#project_path = os.path.join(Config["ProjectRoot"], project_name)
	project_path = Config["ProjectRoot"]
	metefile_path =  "%s.mete" % os.path.join(os.path.split(sys.argv[0])[0], project_name)
	target_type = Config["SyncFileExt"].split(",")
	
	for file_info in g_localfile_map.values():		        #检查是否有文件被删除
		if not os.path.exists(file_info[4]):        
			g_operation_queue.append([file_info[0], file_info[1], file_info[2], file_info[3], file_info[4], file_info[5], "D"])    
			del g_localfile_map[file_info[1]]
			g_rebuild_makefile = True

	for root, _, files in os.walk(project_path, True):   
		for file_name in files:
			ext_name = os.path.splitext(file_name)[-1].lower()
		
			if target_type.count(ext_name) == 0:
				continue
		
			file_full_path = os.path.join(root, file_name)
			#file_rel_path = root[len(project_path):]
			#file_rel_path = root[len(Config["ProjectRoot"]):]
			file_rel_path = "\\" + project_name + root[len(Config["ProjectRoot"]):]
			file_key = file_full_path.__hash__()
			file_lastmoditime = int(os.stat(file_full_path).st_mtime)            
		
			if g_localfile_map.has_key(file_key):           #老文件,检查是否有更改 
				local_fileinfo = g_localfile_map[file_key]
		
				if local_fileinfo[2] != file_lastmoditime:
					g_operation_queue.append(["F", file_key, file_lastmoditime, file_name, file_full_path, file_rel_path, "M"])                    
					local_fileinfo[2] = file_lastmoditime 
		
					if file_name == Config["CMakeListFileName"]:
						g_rebuild_makefile = True
			else:           #新文件,检查是否需要新建配套文件夹,然后直接新增
				check_if_need_create_dir(root, file_rel_path)         
		
				g_operation_queue.append(["F", file_key, file_lastmoditime, file_name, file_full_path, file_rel_path, "N"])
				g_localfile_map[file_key] = ["F", file_key, file_lastmoditime, file_name, file_full_path, file_rel_path]           
				g_rebuild_makefile = True

def save_mete_file(project_name):
    if not g_operation_queue:
		return

    metefile_path =  "%s.mete" % os.path.join(os.path.split(sys.argv[0])[0], project_name)

    try:
		f = open(metefile_path, "w")
	
		lines = []
	
		for key, fileinfo in g_localfile_map.items():
			line = "%s;%d;%d;%s;%s;%s\n" % (fileinfo[0], fileinfo[1], fileinfo[2], fileinfo[3], fileinfo[4], fileinfo[5])
			lines.append(line)
	
		f.writelines(lines)
	
		f.close()
	
		return True;
    except IOError, e:     
		print e.message
		return False
    except Exception, e:
		print e.message
		return False    

def main():
	project_name = ""
	
	if len(sys.argv) < 2:
		print "Useage:Commond [ProjectName=?] \n If no project name ,will use default project name in config file!"
		project_name = "QCenter"
	else:
		project_name = sys.argv[1]	
				
	if not load_local_env(project_name):
		print "can't load local file info,file sync failed!"
		return

	project_file_check(project_name)

	if file_synv():
		save_mete_file(project_name)

	print "File sync succeed"

if __name__ == '__main__':
    main()
