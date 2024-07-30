#include <CoDrone.h>

// 關卡名稱 :BALLET DANCE//
void setup() {
    CoDrone.begin(115200);  
    CoDrone.pair();          //連接至無人機

    CoDrone.takeoff();           //take off

    CoDrone.setThrottle(25);       //向上飛     
    CoDrone.move(1.5);             //1.5 秒後在執行下一個動作

    for x in range(0, 3, 1):     //執行舞步三次
        CoDrone.setPitch(30);     //前行
        CoDrone.setyaw(100);      //旋轉
    
    CoDrone.setPitch(30);        //前行
    CoDrone.setThrottle(25);     //向上飛
    drone.hover(0.5);            //停留0.5秒
    CoDrone.setThrottle(0);      //回到遠本高度
    CoDrone.land();               //降落
}

void loop () {                      //緊急停止
    byte bt8 = digitalRead(18);
    byte bt4 = digitalRead(14);
    byte bt1 = digitalRead(11);

    if(bt1 && bt4 && !bt8){
        CoDrone.emergencyStop();
    }
}