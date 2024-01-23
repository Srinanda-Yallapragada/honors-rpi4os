#include "fb.h"
#include "io.h"

void main() {
  uart_init();
  fb_init();

  drawString(90, 90, "Hello Prof Tim!", 0x0f);

  drawString(100, 110,
             "                    "
             "&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&&%    ",
             0x0f);
  drawString(100, 120,
             "                   &&&                      ,&&&                 "
             "      &&     ",
             0x0f);
  drawString(100, 130,
             "                   &&  *%%%%%%%%%%%%%%%%%&  @&&*  "
             "%%%%%%%%%%%%%%%%%&  @&#     ",
             0x0f);
  drawString(100, 140,
             "                  &&&  %%%%%%%%%%%%%%%%%%  *&&@  "
             "%%%%%%%%%%%%%%%%%%,  &&      ",
             0x0f);
  drawString(100, 150,
             "                  &&  *%%%%%%%%%%%%%%%%%@  @&&,  "
             "%%%%%%%%%%%%%%%%%&  &&(      ",
             0x0f);
  drawString(100, 160,
             "                 &&(  %%%%%%%%%%%%%%%%%%  *&&@  "
             "%%%%%%%%%%%%%%%%%%,  &&       ",
             0x0f);
  drawString(100, 170,
             "                 &&      %%%%%%%%%%%%%%#  &&&. .%%%%%%%%%%%%%%,  "
             "   &&/       ",
             0x0f);
  drawString(100, 180,
             "       &&&&&&&&&&&&&&@  /%&&&&&&&&&&&%%  /&&&  %%%%%%%%%%%%%%&  "
             "@&&&&&        ",
             0x0f);
  drawString(100, 190,
             "      .&(      **%&&&% .&%       *%&%%/  &&&  ,%%%%%%%%%%%%%%  "
             ".&@            ",
             0x0f);
  drawString(100, 200,
             "      &%     *.***%&& &%   "
             "*,****/%&&&@@&&&&@@&%%%%%&&&&&&&&&@@&&@@@@@@@@@@@/ ",
             0x0f);
  drawString(100, 210,
             "     .#&%   ** .***%&%   **  ***%&&&&*      .(&%&%/         ,&%. "
             "        *%@  ",
             0x0f);
  drawString(100, 220,
             "      .&/   **  .**/   ,*,  ***%&%&&/  **  ,*(&&%   **//****,%  "
             ",**//****/&,  ",
             0x0f);
  drawString(100, 230,
             "      &%   ***/  .*  ,**#  .**/%&&&/  **%  **(&&%  **(%%%%%%%,  "
             "**%%%%%%&&&   ",
             0x0f);
  drawString(100, 240,
             "     *%,  .**(%/  .***%%   ***%&%&(  **%%  **#&&( **,,,,, "
             ",*%..**,,,,, **%%   ",
             0x0f);
  drawString(100, 250,
             "   *&&%   ***%&&# ,*&&&(  .**(&&&(  .      **#&%%%%%%%%  "
             ",**%%%%%%%%  **(&.   ",
             0x0f);
  drawString(100, 260,
             "   @%    ,**(&.(&%%&&&%   ****(.   **#%%(  ***#,         **%     "
             "    .*,%&    ",
             0x0f);
  drawString(100, 270,
             "  /%,,******%% &&&#.%&(.******% ****/%&% .****% "
             "**********%#.*********/%&     ",
             0x0f);
  drawString(100, 280,
             "  ########### (&%  "
             "%&&&&&&&&&&&&&&########&&&&&&&&&&&&&&###&&########(        ",
             0x0f);
  drawString(100, 290,
             "              &&  *%%%%%%%%%%%%%%*,,,,,,,%%%%%%%%%%%%%%&  &&%    "
             "             ",
             0x0f);
  drawString(100, 300,
             "             /&%  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  .&&     "
             "             ",
             0x0f);
  drawString(100, 310,
             "             /&@  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%  *&&      "
             "             ",
             0x0f);
  drawString(100, 320,
             "              @&*  %%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%&   &&%       "
             "             ",
             0x0f);
  drawString(100, 330,
             "               &&@   *&%%%%%%%%%%%%%%%%%%%%%%%%&*   *&&&         "
             "             ",
             0x0f);
  drawString(100, 340,
             "                 %&&&/.                        .(&&&&            "
             "             ",
             0x0f);
  drawString(100, 350,
             "                      #@&&&&&&&&&&&&&&&&&&&&&&@(                 "
             "             ",
             0x0f);

  while (1)
    ;
}