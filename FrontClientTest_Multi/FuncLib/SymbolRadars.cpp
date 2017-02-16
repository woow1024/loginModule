#include "StdAfx.h"
#include "SymbolRadars.h"
#include "SqliteWrap.h"
#include "GlobalSetting.h"
#include "FuncLib.h"

MHSymbolRadar::MHSymbolRadar(LPCTSTR lpSymbol/* = NULL*/)
{
	if (lpSymbol != NULL)
	{
		m_strSymbol = lpSymbol;
		InitRadar();
	}
}

BOOL MHSymbolRadar::IsTriggerLastPrice( RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack )
{
	if (NULL == pPack)
		return FALSE;
	BOOL bTrigger(FALSE);
	if (radacon.strType == _T("≥"))
		bTrigger = pPack->data.dbClose >= radacon.dValue ? TRUE : FALSE;
	else if (radacon.strType == _T("≤"))
		bTrigger = pPack->data.dbClose <= radacon.dValue ? TRUE : FALSE;

	if (bTrigger)
	{
		radahis.dValue = radacon.dValue;
		radahis.strName = radacon.strName;
		radahis.strType = radacon.strType;
		radahis.tTime = radacon.tTime;
		radahis.tActive = time(NULL);
		radahis.strUnit = radacon.strUnit;
		radahis.dActiveValue = pPack->data.dbClose;
	}
	return bTrigger;
}

BOOL MHSymbolRadar::IsTriggerBuy( RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack )
{
	if (NULL == pPack)
		return FALSE;
	BOOL bTrigger(FALSE);
	if (radacon.strType == _T("≥"))
		bTrigger = pPack->data.dbBuyPrice[0].GetValue() >= radacon.dValue ? TRUE : FALSE;
	else if (radacon.strType == _T("≤"))
		bTrigger = pPack->data.dbBuyPrice[0].GetValue() <= radacon.dValue ? TRUE : FALSE;

	if (bTrigger)
	{
		radahis.dValue = radacon.dValue;
		radahis.strName = radacon.strName;
		radahis.strType = radacon.strType;
		radahis.tTime = radacon.tTime;
		radahis.tActive = time(NULL);
		radahis.strUnit = radacon.strUnit;
		radahis.dActiveValue = pPack->data.dbBuyPrice[0].GetValue();
	}
	return bTrigger;
}

BOOL MHSymbolRadar::IsTriggerSell( RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack )
{
	if (NULL == pPack)
		return FALSE;
	BOOL bTrigger(FALSE);
	if (radacon.strType == _T("≥"))
		bTrigger = pPack->data.dbSellPrice[0].GetValue() >= radacon.dValue ? TRUE : FALSE;
	else if (radacon.strType == _T("≤"))
		bTrigger = pPack->data.dbSellPrice[0].GetValue() <= radacon.dValue ? TRUE : FALSE;

	if (bTrigger)
	{
		radahis.dValue = radacon.dValue;
		radahis.strName = radacon.strName;
		radahis.strType = radacon.strType;
		radahis.tTime = radacon.tTime;
		radahis.tActive = time(NULL);
		radahis.strUnit = radacon.strUnit;
		radahis.dActiveValue = pPack->data.dbSellPrice[0].GetValue();
	}
	return bTrigger;
}

BOOL MHSymbolRadar::IsTriggerRiseRange( RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack )
{
	if (NULL == pPack)
		return FALSE;
	BOOL bTrigger(FALSE);
	if (pPack->data.dbChangeRate >= 0 )
	{
		if (radacon.strType == _T("≥"))
			bTrigger = pPack->data.dbChangeRate.GetValue() * 100 >= radacon.dValue ? TRUE : FALSE;
		else if (radacon.strType == _T("≤"))
			bTrigger = pPack->data.dbChangeRate.GetValue() * 100 <= radacon.dValue ? TRUE : FALSE;
	}

	if (bTrigger)
	{
		radahis.dValue = radacon.dValue;
		radahis.strName = radacon.strName;
		radahis.strType = radacon.strType;
		radahis.tTime = radacon.tTime;
		radahis.tActive = time(NULL);
		radahis.strUnit = radacon.strUnit;
		radahis.dActiveValue = pPack->data.dbChangeRate.GetValue()*100;
	}
	return bTrigger;
}

BOOL MHSymbolRadar::IsTriggerFallRange( RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack )
{
	if (NULL == pPack)
		return FALSE;
	BOOL bTrigger(FALSE);
	if (pPack->data.dbChangeRate <= 0 )
	{
		if (radacon.strType == _T("≥"))
			bTrigger = fabs(pPack->data.dbChangeRate.GetValue() * 100) >= fabs(radacon.dValue) ? TRUE : FALSE;
		else if (radacon.strType == _T("≤"))
			bTrigger = fabs(pPack->data.dbChangeRate.GetValue() * 100) <= fabs(radacon.dValue) ? TRUE : FALSE;
	}

	if (bTrigger)
	{
		radahis.dValue = radacon.dValue;
		radahis.strName = radacon.strName;
		radahis.strType = radacon.strType;
		radahis.tTime = radacon.tTime;
		radahis.tActive = time(NULL);
		radahis.strUnit = radacon.strUnit;
		radahis.dActiveValue = pPack->data.dbChangeRate.GetValue()*100;
	}
	return bTrigger;
}

BOOL MHSymbolRadar::IsTriggerVolNow( RADARCON& radacon,RADARHIS& radahis, GWSnapShotPack* pPack )
{
	if (NULL == pPack)
		return FALSE;
	BOOL bTrigger(FALSE);
	if (radacon.strType == _T("≥"))
		bTrigger = pPack->data.dbVolumeNow.GetValue() >= radacon.dValue ? TRUE : FALSE;
	else if (radacon.strType == _T("≤"))
		bTrigger = pPack->data.dbVolumeNow.GetValue() <= radacon.dValue ? TRUE : FALSE;

	if (bTrigger)
	{
		radahis.dValue = radacon.dValue;
		radahis.strName = radacon.strName;
		radahis.strType = radacon.strType;
		radahis.tTime = radacon.tTime;
		radahis.tActive = time(NULL);
		radahis.strUnit = radacon.strUnit;
		radahis.dActiveValue = pPack->data.dbVolumeNow.GetValue();
	}
	return bTrigger;
}

BOOL MHSymbolRadar::IsTriggerCondition( GWSnapShotPack* pPack,RADARHISTORYVEC& radahisVec )
{
	radahisVec.clear();
	if (pPack->pSymbolObj->GetObject() != m_strSymbol)
		return FALSE;
	RADARCONTIONVECITR itrCon = m_radarVec.begin();
	for (itrCon = m_radarVec.begin(); itrCon != m_radarVec.end(); itrCon++)
	{
		BOOL bTrigger(FALSE);
		RADARHIS radahis;
		if ((*itrCon).nActive == 0)
			continue;

		if ((*itrCon).strName == _T("最新价"))
		{
			if (IsTriggerLastPrice((*itrCon),radahis,pPack))
				radahisVec.push_back(radahis);
		}
		else if ((*itrCon).strName == _T("现量"))
		{
			if (IsTriggerVolNow((*itrCon),radahis,pPack))
				radahisVec.push_back(radahis);
		}
		else if ((*itrCon).strName == _T("买入价"))
		{
			if (IsTriggerBuy((*itrCon),radahis,pPack))
				radahisVec.push_back(radahis);
		}
		else if ((*itrCon).strName == _T("卖出价"))
		{
			if (IsTriggerSell((*itrCon),radahis,pPack))
				radahisVec.push_back(radahis);
		}
		else if ((*itrCon).strName == _T("涨幅"))
		{
			if (IsTriggerRiseRange((*itrCon),radahis,pPack))
				radahisVec.push_back(radahis);
		}
		else if ((*itrCon).strName == _T("跌幅"))
		{
			if (IsTriggerFallRange((*itrCon),radahis,pPack))
				radahisVec.push_back(radahis);
		}
	}
	return TRUE;
}

void MHSymbolRadar::InitRadar()
{
	InitRadarCons();
	InitRadarHis();
	InitRadarRing();
}

void MHSymbolRadar::InitRadarCons()
{
	m_radarVec.clear();
	CString strRadaDB = GetGloablSetting()->GetGFSubDir(GDT_USERDATA) + _T("gwradar.db");
	if (FALSE == GW_PathFile::IsPathFileExist(strRadaDB)) return ;

	SqliteWrap sqlite;
	CString strSQL;
	if (sqlite.Open(strRadaDB,FALSE) == TRUE)
	{
		strSQL.Format(_T("SELECT * FROM radars WHERE symobj = '%s' "),m_strSymbol.MakeUpper());
		sqlite.BeginTransaction();
		SqliteQueryTask task(&sqlite);
		// 获取板块划分
		if (task.TaskBegin((LPCTSTR)strSQL) == FALSE) 
		{
			m_radarVec.clear();
			sqlite.RollbackTransaction();
			return ;
		}
		while (task.MoveNextRow())
		{
			RADARCON radacon;
			radacon.lID = task.ValueAsInt(0);
			radacon.strName = task.ValueAsString(2);
			radacon.strType = task.ValueAsString(3);
			radacon.dValue = task.ValueAsDouble(4);
			radacon.tTime = task.ValueAsInt(5);
			radacon.nActive = task.ValueAsInt(6);
			radacon.strUnit = task.ValueAsString(7);
			m_radarVec.push_back(radacon);
		}
		task.TaskFinish();
		sqlite.CommitTransaction();
	}
}

void MHSymbolRadar::InitRadarHis()
{
	m_radarHisVes.clear();
	CString strRadaDB = GetGloablSetting()->GetGFSubDir(GDT_USERDATA) + _T("gwradar.db");
	if (FALSE == GW_PathFile::IsPathFileExist(strRadaDB)) return;

	SqliteWrap sqlite;
	CString strSQL;
	if (sqlite.Open(strRadaDB,FALSE) == TRUE)
	{
		sqlite.BeginTransaction();
		strSQL.Format(_T("SELECT * FROM radarshis WHERE symobj = '%s' ORDER BY activetime DESC LIMIT 30"),m_strSymbol.MakeUpper());
		SqliteQueryTask task(&sqlite);
		// 获取板块划分
		if (task.TaskBegin((LPCTSTR)strSQL) == FALSE) 
		{
			sqlite.RollbackTransaction();
			m_radarHisVes.clear();
			return ;
		}
		while (task.MoveNextRow())
		{
			RADARHIS radarhis;
			radarhis.lID = task.ValueAsInt(0);
			radarhis.strName = task.ValueAsString(2);
			radarhis.strType = task.ValueAsString(3);
			radarhis.dValue = task.ValueAsDouble(4);
			radarhis.tTime = task.ValueAsInt(5);
			radarhis.tActive = task.ValueAsInt(6);
			radarhis.strUnit = task.ValueAsString(7);
			m_radarHisVes.push_back(radarhis);
		}
		task.TaskFinish();
		sqlite.CommitTransaction();
	}
}

void MHSymbolRadar::SetSymbol( CString strSymbol )
{
	m_strSymbol = strSymbol;
	InitRadar();
}

BOOL MHSymbolRadar::FindRadarCon( CString strSymbol,CString strName,RADARCON& radars )
{
	BOOL bRes(FALSE);
	if (strSymbol != m_strSymbol) return bRes;

	CString strTrimName = strName;
	strTrimName.Replace(_T(" "),_T(""));
	RADARCONTIONVECITR itr;
	for (itr = m_radarVec.begin(); itr != m_radarVec.end(); itr ++)
	{
		if ((*itr).strName == strTrimName)
		{
			radars = (*itr);
			bRes = TRUE;
			break;
		}
	}

	return bRes;
}

RADARHISTORYVEC MHSymbolRadar::GetRadarsHis()
{
	return m_radarHisVes;
}

void MHSymbolRadar::GetWarnSymbols(CSTRINGVEC& symVec)
{
	symVec.clear();
	CString strRadaDB = GetGloablSetting()->GetGFSubDir(GDT_USERDATA) + _T("gwradar.db");
	if (FALSE == GW_PathFile::IsPathFileExist(strRadaDB)) return ;
	SqliteWrap sqlite;
	CString strSQL;
	if (sqlite.Open(strRadaDB,FALSE) == TRUE)
	{
		strSQL.Format(_T("SELECT symobj FROM radars WHERE active = 1 GROUP BY symobj "));
		sqlite.BeginTransaction();
		SqliteQueryTask task(&sqlite);
		// 获取板块划分
		if (task.TaskBegin((LPCTSTR)strSQL) == FALSE) 
		{
			sqlite.RollbackTransaction();
			return ;
		}
		while (task.MoveNextRow())
		{
			CString strSymobj = task.ValueAsString(0);
			symVec.push_back(strSymobj);
		}
		task.TaskFinish();
		sqlite.CommitTransaction();
	}
	return ;
}

BOOL MHSymbolRadar::IsSymbolWarn( SymbolObj* pSymobj )
{
	if (pSymobj == NULL) return FALSE;

	BOOL bExistsRadars(FALSE);
	CString strRadaDB = GetGloablSetting()->GetGFSubDir(GDT_USERDATA) + _T("gwradar.db");
	if (FALSE == GW_PathFile::IsPathFileExist(strRadaDB)) return FALSE;
	SqliteWrap sqlite;
	CString strSQL;
	if (sqlite.Open(strRadaDB,FALSE) == TRUE)
	{
		sqlite.BeginTransaction();
		CString strSymobj = pSymobj->GetObject();
		strSQL.Format(_T("SELECT COUNT(ID) FROM radars WHERE active = 1 and symobj = '%s' "),strSymobj.MakeUpper());
		SqliteQueryTask task(&sqlite);
		task.TaskBegin((LPCTSTR)strSQL);
		while (task.MoveNextRow())
		{
			int nCount = task.ValueAsInt(0);
			if (nCount > 0)
			{
				bExistsRadars = TRUE;
				break;
			}
		}
		task.TaskFinish();
		sqlite.CommitTransaction();
	}
	return bExistsRadars;
}

BOOL MHSymbolRadar::InvalidSymbolWarn( SymbolObj* pSymobj )
{
	BOOL bResult(FALSE);
	if (pSymobj == NULL) return bResult;

	CString strRadaDB = GetGloablSetting()->GetGFSubDir(GDT_USERDATA) + _T("gwradar.db");
	if (FALSE == GW_PathFile::IsPathFileExist(strRadaDB)) return FALSE;
	SqliteWrap sqlite;
	CString strSQL;
	if (sqlite.Open(strRadaDB,FALSE) == TRUE)
	{
		sqlite.BeginTransaction();
		CString strSymobj = pSymobj->GetObject();
		strSQL.Format(_T("UPDATE radars set active = 0 WHERE symobj = '%s' "),strSymobj.MakeUpper());
		bResult = sqlite.Exec(strSQL);
		if (bResult)
			sqlite.CommitTransaction();
		else
			sqlite.RollbackTransaction();
	}
	return bResult;
}


CString MHSymbolRadar::GetSymbolObj()
{
	return m_strSymbol;
}

BOOL MHSymbolRadar::SaveRadarsSetting( RADARCONTIONVEC& radarsVec )
{
	//保存雷达预警配置
	CString strRadaDB = GetGloablSetting()->GetGFSubDir(GDT_USERDATA) + _T("gwradar.db");
	BOOL bExitDBFile = GW_PathFile::IsPathFileExist(strRadaDB);
	SqliteWrap sqlite;
	if (sqlite.Open(strRadaDB,!bExitDBFile) == FALSE)
	{
//		SysMessageBox(_T("保存雷达预警失败[数据打开失败]"),_T("提示"));
		WriteDebugMessage(_T("保存预警设置失败[数据打开失败]"));
		return FALSE;
	}
	CString strSQL = _T("CREATE TABLE IF NOT EXISTS radars( \
						\"ID\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ON CONFLICT ROLLBACK,\
						\"symobj\"  TEXT(30) NOT NULL ON CONFLICT ROLLBACK,\
						\"raname\"  TEXT(50) NOT NULL ON CONFLICT ROLLBACK,\
						\"ratype\" TEXT(20) NOT NULL ON CONFLICT ROLLBACK,\
						\"value\"  REAL(12,6) NOT NULL ON CONFLICT ROLLBACK,\
						\"settime\"  INTEGER NOT NULL ON CONFLICT ROLLBACK,\
						\"active\" INTEGER NOT NULL ON CONFLICT ROLLBACK,\
						\"unit\" TEXT(20) NOT NULL ON CONFLICT ROLLBACK\
						)");
	if (sqlite.Exec((LPCTSTR)strSQL) == FALSE)
	{
//		SysMessageBox(_T("保存预警设置失败[表创建失败]"),_T("提示"));
		WriteDebugMessage(_T("保存预警设置失败[表创建失败]"));
		return FALSE;
	}

	sqlite.BeginTransaction();
	RADARCONTIONVECITR itr = radarsVec.begin();
	for (itr = radarsVec.begin(); itr != radarsVec.end(); itr ++)
	{
		SqliteQueryTask task(&sqlite);
		strSQL.Format(_T("select count(ID)  from radars where symobj = '%s' and raname = '%s'"),m_strSymbol.MakeUpper(),(*itr).strName);
		task.TaskBegin((LPCTSTR)strSQL);
		BOOL bExistsRadars(FALSE);
		while (task.MoveNextRow())
		{
			int nCount = task.ValueAsInt(0);
			if (nCount > 0)
			{
				bExistsRadars = TRUE;
				break;
			}
		}
		task.TaskFinish();

		if (bExistsRadars)
		{
			strSQL.Format(_T("UPDATE radars set value = '%f' ,ratype = '%s',settime = '%I64d',active = '%d' ,unit = '%s' where raname = '%s' and symobj = '%s' "),
				(*itr).dValue,(*itr).strType,(*itr).tTime,(*itr).nActive,(*itr).strUnit,(*itr).strName,m_strSymbol.MakeUpper());
		}
		else
		{
			strSQL.Format(_T("INSERT INTO radars(symobj,raname,ratype,value,settime,active,unit) VALUES('%s','%s','%s','%f','%I64d','%d','%s')"),
				m_strSymbol.MakeUpper(),(*itr).strName,(*itr).strType,(*itr).dValue,(*itr).tTime,(*itr).nActive,(*itr).strUnit);
		}

		sqlite.Exec(strSQL);
	}
	sqlite.CommitTransaction();
	return TRUE;
}

BOOL MHSymbolRadar::SaveRadarsHis( RADARHISTORYVEC& radarhisVec )
{
	//保存雷达预警配置
	CString strRadaDB = GetGloablSetting()->GetGFSubDir(GDT_USERDATA) + _T("gwradar.db");
	BOOL bExitDBFile = GW_PathFile::IsPathFileExist(strRadaDB);
	SqliteWrap sqlite;
	if (sqlite.Open(strRadaDB,!bExitDBFile) == FALSE)
	{
//		SysMessageBox(_T("保存雷达预警失败[数据打开失败]"),_T("提示"));
		WriteDebugMessage(_T("保存雷达预警失败[数据打开失败]"));
		return FALSE;
	}
	CString strSQL = _T("CREATE TABLE IF NOT EXISTS radarshis( \
						\"ID\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ON CONFLICT ROLLBACK,\
						\"symobj\"  TEXT(30) NOT NULL ON CONFLICT ROLLBACK,\
						\"raname\"  TEXT(50) NOT NULL ON CONFLICT ROLLBACK,\
						\"ratype\" TEXT(20) NOT NULL ON CONFLICT ROLLBACK,\
						\"value\"  REAL(12,6) NOT NULL ON CONFLICT ROLLBACK,\
						\"settime\"  INTEGER NOT NULL ON CONFLICT ROLLBACK,\
						\"activetime\"  INTEGER NOT NULL ON CONFLICT ROLLBACK,\
						\"unit\" TEXT(20) NOT NULL ON CONFLICT ROLLBACK,\
						\"activevalue\" REAL(12,6) NOT NULL ON CONFLICT ROLLBACK\
						)");
	if (sqlite.Exec((LPCTSTR)strSQL) == FALSE)
	{
//		SysMessageBox(_T("保存雷达预警失败[表创建失败]"),_T("提示"));
		WriteDebugMessage(_T("保存雷达预警失败[表创建失败]"));
		return FALSE;
	}

	sqlite.BeginTransaction();
	RADARHISTORYVECITR itr = radarhisVec.begin();
	for (itr = radarhisVec.begin(); itr != radarhisVec.end(); itr ++)
	{
		strSQL.Format(_T("INSERT INTO radarshis(symobj,raname,ratype,value,settime,activetime,unit,activevalue) VALUES('%s','%s','%s','%f','%I64d','%I64d','%s',%f)"),
				m_strSymbol.MakeUpper(),(*itr).strName,(*itr).strType,(*itr).dValue,(*itr).tTime,(*itr).tActive,(*itr).strUnit,(*itr).dActiveValue);
		sqlite.Exec(strSQL);

		strSQL.Format(_T("UPDATE radars SET active = %d WHERE symobj = '%s' AND raname = '%s'"),FALSE,m_strSymbol.MakeUpper(),(*itr).strName);
		sqlite.Exec(strSQL);
	}
	sqlite.CommitTransaction();
	return TRUE;
}

BOOL MHSymbolRadar::IsActiveRadar()
{
	BOOL bRes(FALSE);

	RADARCONTIONVECITR itr;
	for (itr = m_radarVec.begin(); itr != m_radarVec.end(); itr ++)
	{
		if ((*itr).nActive)
		{
			bRes = TRUE;
			break;
		}
	}

	return bRes;
}

void MHSymbolRadar::InitRadarRing()
{
	m_bRing = TRUE;
	CString strRadaDB = GetGloablSetting()->GetGFSubDir(GDT_USERDATA) + _T("gwradar.db");
	if (FALSE == GW_PathFile::IsPathFileExist(strRadaDB)) return ;

	SqliteWrap sqlite;
	CString strSQL;
	if (sqlite.Open(strRadaDB,FALSE) == TRUE)
	{
		strSQL.Format(_T("SELECT ring FROM radarswav WHERE symobj = '%s' LIMIT 1"),m_strSymbol.MakeUpper());
		sqlite.BeginTransaction();
		SqliteQueryTask task(&sqlite);
		// 获取板块划分
		if (task.TaskBegin((LPCTSTR)strSQL) == FALSE) 
		{
			sqlite.RollbackTransaction();
			return ;
		}
		while (task.MoveNextRow())
		{
			m_bRing = task.ValueAsInt(0);
		}
		task.TaskFinish();
		sqlite.CommitTransaction();
	}
}

BOOL MHSymbolRadar::SaveRadarRing( BOOL bRing)
{
	//保存雷达预警配置
	CString strRadaDB = GetGloablSetting()->GetGFSubDir(GDT_USERDATA) + _T("gwradar.db");
	BOOL bExitDBFile = GW_PathFile::IsPathFileExist(strRadaDB);
	SqliteWrap sqlite;
	if (sqlite.Open(strRadaDB,!bExitDBFile) == FALSE)
	{
		WriteDebugMessage(_T("保存雷达预警失败[数据打开失败]"));
		return FALSE;
	}
	CString strSQL = _T("CREATE TABLE IF NOT EXISTS radarswav( \
						\"ID\" INTEGER PRIMARY KEY AUTOINCREMENT NOT NULL ON CONFLICT ROLLBACK,\
						\"ring\"  INTEGER NOT NULL ON CONFLICT ROLLBACK,\
						\"symobj\"  TEXT(50) NOT NULL ON CONFLICT ROLLBACK\
						)");
	if (sqlite.Exec((LPCTSTR)strSQL) == FALSE)
	{
		WriteDebugMessage(_T("保存雷达预警失败[表创建失败]"));
		return FALSE;
	}

	sqlite.BeginTransaction();

	SqliteQueryTask task(&sqlite);
	strSQL.Format(_T("select count(ID)  from radarswav where symobj = '%s' "),m_strSymbol.MakeUpper());
	task.TaskBegin((LPCTSTR)strSQL);
	BOOL bExistsRadars(FALSE);
	while (task.MoveNextRow())
	{
		int nCount = task.ValueAsInt(0);
		if (nCount > 0)
		{
			bExistsRadars = TRUE;
			break;
		}
	}
	task.TaskFinish();

	if (bExistsRadars)
	{
		strSQL.Format(_T("UPDATE radarswav set ring = '%d' where symobj = '%s' "),bRing,m_strSymbol.MakeUpper());
	}
	else
	{
		strSQL.Format(_T("INSERT INTO radarswav(symobj,ring) VALUES('%s','%d')"),m_strSymbol.MakeUpper(),bRing);
	}

	sqlite.Exec(strSQL);
	sqlite.CommitTransaction();
	return TRUE;
}

BOOL MHSymbolRadar::IsAlarmSound()
{
	return m_bRing;
}
