<#
  功能：批量将当前目录 + 所有子目录内的文件编码从 UTF-8 转换为 GBK
  注意：会覆盖原文件！转换前请务必备份！
#>

# 获取脚本所在目录（根目录）
$rootPath = $PSScriptRoot

# 获取脚本自身名称（避免转换自己）
$scriptName = $MyInvocation.MyCommand.Name

# 获取 根目录 + 所有子目录 下的所有文件
$files = Get-ChildItem -Path $rootPath -File -Recurse

Write-Host "`n开始扫描目录：$rootPath`n总共找到 $($files.Count) 个文件`n" -ForegroundColor Cyan

# 遍历转换
foreach ($file in $files) {
    # 跳过脚本自己
    if ($file.Name -eq $scriptName) {
        Write-Host "⏭️ 跳过脚本文件：$($file.FullName)"
        continue
    }

    try {
        # UTF-8 读取
        $content = Get-Content -Path $file.FullName -Encoding UTF8 -Raw -ErrorAction Stop

        # GBK 写入（覆盖原文件）
        Set-Content -Path $file.FullName -Encoding GBK -Value $content -ErrorAction Stop

        Write-Host "✅ 转换成功：$($file.FullName)" -ForegroundColor Green
    }
    catch {
        Write-Host "❌ 转换失败：$($file.FullName)`n   错误：$_" -ForegroundColor Red
    }
}

Write-Host "`n🎉 所有文件处理完成！" -ForegroundColor Cyan
Read-Host -Prompt "`n按回车键退出"