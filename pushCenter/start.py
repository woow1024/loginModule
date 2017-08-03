import os
os.system("celery -A Push worker  --loglevel=info") 