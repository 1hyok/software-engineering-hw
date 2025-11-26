#!/usr/bin/env python3
"""
프로젝트를 .zip 파일로 압축하는 스크립트
"""

import os
import zipfile
from datetime import datetime

def create_project_zip():
    zip_filename = f"RVC_Project_Test_{datetime.now().strftime('%Y%m%d_%H%M%S')}.zip"
    
    # 포함할 디렉토리 및 파일
    include_paths = [
        'src/',
        'tests/',
        '1.c',
        '2.c',
        'README.md',
        'RVC_Test_Documentation.docx',
    ]
    
    # 제외할 파일/디렉토리
    exclude_patterns = [
        '__pycache__',
        '.pyc',
        '.exe',
        '.o',
        '.obj',
    ]
    
    with zipfile.ZipFile(zip_filename, 'w', zipfile.ZIP_DEFLATED) as zipf:
        for path in include_paths:
            if os.path.exists(path):
                if os.path.isdir(path):
                    for root, dirs, files in os.walk(path):
                        # 제외 패턴 필터링
                        dirs[:] = [d for d in dirs if not any(pat in d for pat in exclude_patterns)]
                        
                        for file in files:
                            if not any(pat in file for pat in exclude_patterns):
                                file_path = os.path.join(root, file)
                                arcname = os.path.relpath(file_path, '.')
                                zipf.write(file_path, arcname)
                                print(f"Added: {arcname}")
                else:
                    zipf.write(path, path)
                    print(f"Added: {path}")
    
    print(f"\n압축 완료: {zip_filename}")
    print(f"파일 크기: {os.path.getsize(zip_filename) / 1024:.2f} KB")

if __name__ == '__main__':
    create_project_zip()

