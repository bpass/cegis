@echo off

if %1 == %nothing% goto end
if %1 == clean goto clean

mkdir %1\DiffUnfiltered
mkdir %1\Diff_10DF
mkdir %1\Diff_10VMF
mkdir %1\Diff_20DF
mkdir %1\Diff_20VMF
mkdir %1\Diff_30DF
mkdir %1\Diff_30VMF
mkdir %1\Diff_40DF
mkdir %1\Diff_40VMF
mkdir %1\Diff_50DF
mkdir %1\Diff_50VMF
goto end

:clean
rmdir /S /Q %2

:end

echo.Done

