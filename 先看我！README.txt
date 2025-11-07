1、如果要运行，直接双击打开 .\EWARM\Project.eww

2、如果要通过Cubemx修改其他配置，**请先备份!!**，然后再进行修改。在点击“Generate Code”后先别打开工程，要把备份中的.Inc\GUI_App.h和.Src\GUI.App.c复制到当前工程中，然后再通过上面第1点的方式打开工程。

3、如果IAR出现无法打开iar_cortexM7ls_math.a的问题，应该先移除iar_cortexM7ls_math.a,然后再手动添加一遍iar_cortexM7ls_math.a