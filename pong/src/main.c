#include <tice.h>
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include <graphx.h>
#include <keypadc.h>
//#include "/Users/squijano/ticeinc/tigcclib.h"
#include <debug.h>

/*
 *--------------------------------------
 * Program Name: pong
 * Author: nicolas quijano
 * License:
 * Description:
 *--------------------------------------
*/

void resetBall();

const int halfHeight = LCD_HEIGHT / 2;
const int quarterHeight = LCD_HEIGHT / 4;
const int halfWidth = LCD_WIDTH / 2;

int paddleLeftY = quarterHeight;
int paddleRightY = quarterHeight;
int paddleHeight = quarterHeight;
int paddleWidth = 5;

int ballX = halfWidth;
int ballY = halfHeight;
int ballRise = 1;
int ballRun = 1;
int ballSize = 10;

int reflection;
int didTouch = 0;

int leftScore = 0;
int rightScore = 0;

int gameOver = 0;
// #region DRAW + CLEAR
void draw()
{
    /*
    a = data     |       b = data
    a = data     |       b = null
    a = null     |       b = data

    */
    //gfx_ZeroScreen();
    //gfx_SwapDraw();
    //gfx_SetDrawBuffer();
    ///gfx_Wait();
    gfx_BlitBuffer();
}

void clear()
{
    gfx_ZeroScreen();
}

void clearBuff()
{
    gfx_SetColor(0);
    gfx_FillRectangle_NoClip(0, 0, LCD_WIDTH, LCD_HEIGHT);
    gfx_SetColor(7);
    //draw balck rectangle and reset color index
}
// #endregion

// #region CHECKS

void checkKeys()
{
    kb_Scan();
    //dbg_sprintf(dbgout,"check");
    //if (kb_IsDown(kb_2nd))
    if (kb_Data[1] & kb_2nd)
    {
        paddleLeftY -= 3;
        //dbg_sprintf(dbgout, "\nup %i : %i", paddleLeftY, paddleLeftY + quarterHeight);
    }
    //else if (kb_IsDown(kb_Alpha))
    else if (kb_Data[2] & kb_Alpha)
    {
        paddleLeftY += 3;
        //dbg_sprintf(dbgout, "\ndown %i : %i", paddleLeftY, paddleLeftY + quarterHeight);
    }

    //if (kb_IsDown(kb_Up))
    if (kb_Data[7] & kb_Up)
    {
        paddleRightY -= 3;
        //dbg_sprintf(dbgout, "\nup %i : %i", paddleLeftY, paddleLeftY + quarterHeight);
    }
    //else if (kb_IsDown(kb_Down))
    else if (kb_Data[7] & kb_Down)
    {
        paddleRightY += 3;
        //dbg_sprintf(dbgout, "\ndown %i : %i", paddleLeftY, paddleLeftY + quarterHeight);
    }
}

void checkScores()
{
    if (leftScore > 9 || rightScore > 9)
    {
        leftScore = 0;
        rightScore = 0;
    }
    // #region FAIl
    // if (leftScore > 3)
    // {
    //     clearBuff();
    //     resetBall();
    //     dbg_sprintf(dbgout, "won");
    //     gfx_PrintStringXY("Left Wins!", halfWidth, 20);
    //     gfx_PrintStringXY("click enter to play again", halfWidth, 40);
    //     dbg_sprintf(dbgout, "won2");
    //     int notDone = 1;
    //     while (notDone)
    //     {
    //         dbg_sprintf(dbgout, "\n123");
    //         for (int i = 0; i < 10; i++)
    //         {
    //             clearBuff();
    //         }
    //         if (kb_Data[1] & kb_2nd)
    //         {
    //             notDone = 0;
    //         }
    //     }
    // }
    // else if (rightScore > 3)
    // {
    //     clearBuff();
    //     resetBall();
    //     gfx_PrintStringXY("Right Wins!", halfWidth, 20);
    //     gfx_PrintStringXY("click enter to play again", halfWidth, 40);
    //     while (1)
    //     {
    //         dbg_sprintf(dbgout, "\n123");
    //         if ((kb_Data[6] & kb_Enter))
    //         {
    //             return;
    //         }
    //     }
    // }
    // #endregion
}

// #endregion

// #region GAME ELEMENTS
void paddleLeft()
{
    gfx_FillRectangle(0, paddleLeftY, paddleWidth, paddleHeight);
}

void paddleRight()
{
    gfx_FillRectangle(LCD_WIDTH - paddleWidth, paddleRightY, paddleWidth, paddleHeight);
}

void resetBall()
{
    ballX = halfWidth;
    ballY = halfHeight;
    ballRise = 1;
    ballRun = 1;
}

void calculateReflection()
{
    reflection = paddleLeftY + paddleHeight - ballY;

    if (reflection <= (int)(paddleHeight / 4))
    {
        ballRise = 2;
    }
    else if (reflection <= (int)(paddleHeight / 2))
    {
        ballRise = 1;
    }
    else if (reflection <= (int)(paddleHeight / 2 + paddleHeight / 4))
    {
        ballRise = -1;
    }
    else
    {
        ballRise = -2;
    }

    ballRun = -1 * ballRun;

    //if (randInt(0, 5) == 1)
    //{
    if (ballRun > 0)
    {
        ballRun++;
    }
    else
    {
        ballRun--;
    }
    //}
}

void ball()
{
    ballX += ballRun;
    ballY += ballRise;
    //dbg_sprintf(dbgout, "\n ball y: %i   screen: %i", ballY + 20, LCD_HEIGHT);
    gfx_FillRectangle(ballX, ballY, ballSize, ballSize);

    if (ballY <= 0)
    { //----------touch top
        ballRise = -1 * ballRise;
    }

    else if (ballY + ballSize >= LCD_HEIGHT)
    { //----------touch bottom
        //dbg_sprintf(dbgout, "\nchange\n");
        ballRise = -1 * ballRise;
    }

    if (ballX <= paddleWidth)
    { //----------touch left
        if ((ballY + ballSize >= paddleLeftY) && (ballY <= paddleLeftY + paddleHeight))
        { //if touch
            calculateReflection();
        }
        else
        { // if missed
            rightScore++;
            dbg_sprintf(dbgout, "won0");
            checkScores();
            resetBall();
        }
    }
    else if (ballX + ballSize >= LCD_WIDTH - paddleWidth)
    { //----------touch right
        if ((ballY + ballSize >= paddleRightY) && (ballY <= paddleRightY + paddleHeight))
        { // if touch
            calculateReflection();
        }
        else
        { // if missed
            leftScore++;
            dbg_sprintf(dbgout, "won0");
            checkScores();

            resetBall();
        }
    }
}

void text()
{
    if (leftScore < 10 && rightScore < 10)
    {
        gfx_SetTextXY(halfWidth - 20, 20);
        gfx_PrintInt(leftScore, 1);
        gfx_PrintStringXY("|", halfWidth, 20);
        gfx_SetTextXY(halfWidth + 20, 20);
        gfx_PrintInt(rightScore, 1);
    }
}
// #endregion

void renderLoop()
{
    checkKeys();
        dbg_sprintf(dbgout, "\nch1");
    clearBuff();
        dbg_sprintf(dbgout, "\nch2");
    paddleLeft();
        dbg_sprintf(dbgout, "\nch3");
    paddleRight();
        dbg_sprintf(dbgout, "\nch4");
    ball();
        dbg_sprintf(dbgout, "\nch5");
    text();
        dbg_sprintf(dbgout, "\nch6");
    draw();
        dbg_sprintf(dbgout, "\nch7");
}

void gameInit()
{
    clear();
    while (1)
    {
        renderLoop();

        if ((kb_Data[6] & kb_Clear) || gameOver)
        {
            return;
        }
    }
}

int main(void)
{
    gfx_Begin();         //enter gfx mode
    gfx_SetDrawBuffer(); //set render type
    gfx_SetColor(7);
    gfx_SetTextScale(2, 2);
    gfx_SetTextFGColor(7);

    gameInit();
    gfx_End();
    return 0;
}
