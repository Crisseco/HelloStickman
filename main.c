#include <conio.h>
#include <String.h>

typedef unsigned char u8;
typedef unsigned int u16;
typedef unsigned long u32;
#define addr(address) (*(u8 *)(address))


#define ADDR_REG_SPRAM 0x2003
#define ADDR_SPRAM_DATA 0x2004

#define BTN_A          0x80
#define BTN_B          0x40
#define BTN_SELECT     0x20
#define BTN_START      0x10
#define BTN_UP         0x08
#define BTN_DOWN       0x04
#define BTN_LEFT       0x02
#define BTN_RIGHT      0x01

#define presskey(keys, k) (keys & (k))
#define iskey(keys, k) (key == (k))
#define downkey(k) (okey!=key && (k)&key)
#define upkey(k) (okey!=key && (k)&okey)
#define putSP(n, x, y, t, a) addr(0x2003)=n*4;\
                             addr(0x2004)=y;\
                             addr(0x2004)=t;\
                             addr(0x2004)=a;\
                             addr(0x2004)=x

u8 readJoystick();
void setColor(u8,u8,u8);

void printBackground(char*);

void printHero(u8,char*, u8,u8);
void printEnemy(u8,u8,char);
void handleHit(u8);

void printStickman(u8,u8);

void main()
{
    char* map =
"    |  |                        "\
"    |  |                        "\
"    |  |                        "\
"    |  |                        "\
"    |  |                        "\
"----+  +------------------------"\
"                                "\
"----+  +-------------+  +-------"\
"    |  |             |  |       "\
"    |  |             |  |       "\
"    |  |             |  |       "\
"    |  |             |  +-------"\
"    |  |             |          "\
"    |  |             +----------"\
"    |  |                        "\
"    |  |                        "\
"    |  |                        "\
"    |  |                        "\
"    |  |                        "\
"    |  |                        "\
"----+  |                        "\
"       |                        "\
"-------+                        ";

    u8 keys,x=15,y=10,i=0,hitted=0;
    char enemyLife ='!';
    addr(0x2000) = 0x80;
    addr(0x2001) = 0x1e;

    setColor(0,0,0x9);
    setColor(0,3, 0x18);

    gotoxy(0,0);
    printBackground(map);

    while(++i)
    {
        keys = readJoystick();
        if(presskey(keys, BTN_UP)) --y;
        if(presskey(keys, BTN_DOWN)) ++y;
        if(presskey(keys, BTN_LEFT)) --x;
        if(presskey(keys, BTN_RIGHT)) ++x;

        if(!(i % 3))
        {
            putSP(0,x,y,'O',0);
            putSP(1,x,y+8,'+',0);
        }
        else if(!(i%2))
        {
            putSP(0,x-8,y+16, '/',0);
            putSP(1,x-8,y+8, keys & BTN_B ? '\\':'-',0);
        }
        else
        {
            putSP(0,x+8,y+8, keys & BTN_A ? '/':'-',0);
            putSP(1,x+8,y+16,'\\',0);
        }

        if(x >= 21*8 && x <= 25*8 && y >= 18*8 && y <= 21*8 && keys & (BTN_B | BTN_A))
        {
            if(!hitted)
            {
                hitted = 1;
                if(enemyLife == '!')
                {
                    enemyLife = '3';
                }
                else if(enemyLife == '0')
                {
                    enemyLife = 'X';
                }
                else
                {
                    enemyLife--;
                }
                handleHit(enemyLife);
            }
        }
        else if(enemyLife != 'X')
        {
            hitted=0;
        }

        printEnemy(23,16, enemyLife);

        if(i == 0xFF) i = 0;
    }

    while(1);
}

void handleHit(u8 enemyLife)
{
    switch(enemyLife)
    {
    case '3':

        gotoxy(8,0);
        cprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c", 20,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,18);
        gotoxy(8,1);
        cprintf("%cTu veux te battre, mon%c",14,14);
        gotoxy(8,2);
        cprintf("%cesti?                 %c",14,14);
        gotoxy(8,3);
        cprintf("%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c",17,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,11,8);
        break;
    case '2':
        gotoxy(8,1);
        cprintf("%cOuch! Attention vous  %c",14,14);
        gotoxy(8,2);
        cprintf("%cpourriez me blesser!  %c",14,14);
        break;
    case '1':
        gotoxy(8,1);
        cprintf("%cMonsieur, excusez-moi %c",14,14);
        gotoxy(8,2);
        cprintf("%cJe saigne abondamment!%c",14,14);
        break;
    case '0':
        gotoxy(8,1);
        cprintf("%cA l'aide! Pitie, je ne%c",14,14);
        gotoxy(8,2);
        cprintf("%csens plus mes jambes! %c",14,14);
        break;
    default:
        gotoxy(8,0);
        cprintf("                        ");
        gotoxy(8,1);
        cprintf("                        ");
        gotoxy(8,2);
        cprintf("                        ");
        gotoxy(8,3);
        cprintf("                        ");
        break;

    }
}

void printBackground(char* map)
{
    gotoxy(0,0);
    cprintf(map);
}

void printEnemy(u8 x, u8 y, char life)
{
    gotoxy(x,y);
    cprintf(" %c", life);

    printStickman(x,y+2);
}

void printStickman(u8 x, u8 y)
{
    gotoxy(x,y);
    cprintf(" O ");
    gotoxy(x,y+1);
    cprintf("-+-");
    gotoxy(x,y+2);
    cprintf("/ \\");
}

void setColor(u8 n, u8 m, u8 color)
{
    addr(0x2006) = 0x3f;
    addr(0x2006) = 0x00 + n * 4 + m;
    addr(0x2007) = color;
}

u8 readJoystick()
{
    u8 n = 8, joy_state = 0;
    addr(0x4016)=01;
    addr(0x4016)=00;
    while(n)
    {
        joy_state = (joy_state << 1) | addr(0x4016) & 1;
        --n;
    }
    return joy_state;
}

