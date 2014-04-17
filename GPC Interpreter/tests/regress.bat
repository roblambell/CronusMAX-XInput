mkdir testResults
for %%a in (*.c) do ..\Debug\C-Interpreter %%a - arg1 arg2 arg3 arg4 >testResults\%%~na.expect
