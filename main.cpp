#include <iostream>
#include <windows.h>
#include <windowsx.h>
#include "VecFunctions.h"
#include <stack>

HDC hdc;
HPEN whitePen;
HBRUSH blackBrush;
HBRUSH yellowBrush;
int size{ 20 };
int size_grid{ 20 };

void SetWindow(int Width, int Height);
void DrawGrid(int x, int y);
void DrawPixel(int x, int y);
void DrawPixel(int x, int y,int xSizeGrid,int ySizeGrid);
void DrawPixel(int x, int y, COLORREF color);
void Pixel(int x, int y, COLORREF color);
void DrawLineDDA(int x0, int y0, int x1, int y1);
void DrawCircleBresenham(int xc, int yc, int r);
void Brezenhem(int x0, int y0, int x1, int y1);
void sorttreug(int Ax, int Ay, int Bx, int By, int Cx, int Cy);
void Flood_shape(int x, int y, int fillColor, int boundaryColor);
void NOTmain(int width, int height);
void WriteScrin(wchar_t* screen, int width, int height);

void DrawShape(int x, int y) {
    COLORREF shapeColor = RGB(150, 0, 200);
    HBRUSH brush = CreateSolidBrush(shapeColor);
    SelectObject(hdc, brush);

    Ellipse(hdc,
        x * size_grid,
        (size - y - 1) * size_grid,
        (x + 1) * size_grid,
        (size - y) * size_grid);

}

int main()
{
    int width = size_grid;
    int height = size_grid;
    SetWindow(size* size_grid, size * size_grid);

    HWND hwnd = GetConsoleWindow();
    hdc = GetDC(hwnd);

    blackBrush = GetStockBrush(BLACK_BRUSH);
    yellowBrush = CreateSolidBrush(RGB(255, 255, 0));

    SelectBrush(hdc, blackBrush);
    FloodFill(hdc, 0, 0, RGB(0, 0, 1));

    whitePen = GetStockPen(WHITE_PEN);
    SelectPen(hdc, whitePen);

    int x1 = 1; int y1 = 1; int x2 = 19;  int y2 = 1; int x3 = 10; int y3 = 19;
    int cnt{};
    int r{ 3 };
    int Ox = 4; int Oy = 1;
    int q{};


    std::cout << "What drawing? " << std::endl;
    std::cin >> q;
    if (q == 1 || q == 2)
    {
        std::cout << "x1 y1: " << std::endl;
        std::cin >> x1 >> y1;
        std::cout << "x2 y2: " << std::endl;
        std::cin >> x2 >> y2;
    }
    if (q == 3) {
        std::cout << "Ox Oy r: " << std::endl;
        std::cin >> Ox >> Oy >> r;
    }

    system("cls");
    for (int i = 0; i <= size_grid * (size + 4) / 15; i++) { std::cout << "\n"; }
    while (true) {
        DrawGrid(size_grid, size_grid);

        if (q == 1) DrawLineDDA(x1, y1, x2, y2);
        if (q == 2) Brezenhem(x1, y1, x2, y2);
        if (q == 3) //DrawShape(r, r);
            DrawCircleBresenham(Ox, Oy, r);
    }
    int i;
    std::cin >> i;
    FloodFill(hdc, 100, 100, RGB(0, 0, 1));

    DeleteObject(whitePen);
    ReleaseDC(hwnd, hdc);
}

void SetWindow(int Width, int Height)
{
    _COORD coord;
    coord.X = Width;
    coord.Y = Height;
    _SMALL_RECT Rect;
    Rect.Top = 0;
    Rect.Left = 0;
    Rect.Bottom = Height - 1;
    Rect.Right = Width - 1;
    HANDLE Handle = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleScreenBufferSize(Handle, coord);
    SetConsoleWindowInfo(Handle, TRUE, &Rect);
}
void DrawGrid(int x, int y) {

    SelectObject(hdc, whitePen);
    for (int i = 0; i <= x; i++) {
        MoveToEx(hdc, size * i, 0, NULL);
        LineTo(hdc, size * i, y * size);

    }
    for (int j = 0; j <= y; j++) {
        MoveToEx(hdc, 0, size * j, NULL);
        LineTo(hdc, x * size, size * j);
    }
}
void DrawPixel(int x, int y, int xSizeGrid, int ySizeGrid) {
    if (x >= 1 && x <= size_grid && y >= 1 && y <= size_grid)
    for (int i = (x - 1) * size; i < x * size; i++) {
        for (int j = size * size_grid - (y) * size; j < size * size_grid - (y - 1) * size; j++) {
            if(x >= 0 && y >= 0 && GetPixel(hdc, i, j) != RGB(255, 255, 0)) SetPixel(hdc, i, j, RGB(255, 255, 0));
        }
    }
}
void DrawPixel(int x, int y) {
    if (x >= 1 && y >= 1 && x <= size_grid && y <= size_grid) {
        for (int i = (x - 1) * size; i < x * size; i++) {
            for (int j = size * size_grid - (y)*size; j < size * size_grid - (y - 1) * size; j++) {
                if (i >= 0 && y >= 0) SetPixel(hdc, i, j, RGB(255, 255, 0));
            }
        }
    }
}
void DrawPixel(int x, int y, COLORREF color) {
    if (x >= 1 && y >= 1 && x <= size_grid && y <= size_grid) {
        for (int i = (x - 1) * size; i < x * size; i++) {
            for (int j = size * size_grid - (y)*size; j < size * size_grid - (y - 1) * size; j++) {
                if (i >= 0 && y >= 0) SetPixel(hdc, i, j, color);
            }
        }
    }
}
void Pixel(int x, int y, COLORREF color) {
    SetPixel(hdc, x, y, color);
}

void DrawLineDDA(int x0, int y0, int x1, int y1) {
    int dx = x1 - x0;
    int dy = y1 - y0;
    int steps = std::max(abs(dx), abs(dy));

    float xIncrement = dx / (float)steps;
    float yIncrement = dy / (float)steps;

    float x = x0;
    float y = y0;

    //int lx = x0, ly = y0;

    for (int i = 0; i <= steps; i++) {
        DrawPixel(round(x), round(y));
        x += xIncrement;
        y += yIncrement;
        //if (abs(lx - round(x)) + abs(ly - round(x)) == 2){DrawPixel(lx, round(y));DrawPixel(round(x), round(y));lx = round(x); ly = round(y);}
        //else{DrawPixel(round(x), round(y));lx = round(x); ly = round(y);}
    }
}
void DrawCircleBresenham(int xc, int yc, int r) {
    int x = 0;
    int y = r;
    int d = 3 - 2 * r;

    while (y >= x) {
        DrawPixel(xc + x, yc + y);
        DrawPixel(xc - x, yc + y);
        DrawPixel(xc + x, yc - y);
        DrawPixel(xc - x, yc - y);
        DrawPixel(xc + y, yc + x);
        DrawPixel(xc - y, yc + x);
        DrawPixel(xc + y, yc - x);
        DrawPixel(xc - y, yc - x);

        if (d < 0) {
            d = d + 4 * x + 6;
        }
        else {
            d = d + 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}
void Brezenhem(int x0, int y0, int x1, int y1)
{
    //x0 = size_grid - x0;
    //x1 = size_grid - x1;
    int A, B, sign;
    A = y1 - y0;
    B = x0 - x1;
    if (abs(A) > abs(B))
        sign = 1;
    else
        sign = -1;
    int signa, signb;
    if (A < 0)
        signa = -1;
    else
        signa = 1;
    if (B < 0)
        signb = -1;
    else
        signb = 1;
    int f = 0;
    DrawPixel(x0, y0);
    int lx = x0, ly = y0;
    int x = x0, y = y0;
    if (sign == -1)
    {
        do {
            f += A * signa;
            if (f > 0)
            {
                f -= B * signb;
                y += signa;
            }
            x -= signb;
            if (abs(lx - x) + abs(ly - y) == 2)
            {
                DrawPixel(lx, y);
                DrawPixel(x, y);
                lx = x; ly = y;
            }
            else
            {
                DrawPixel(x, y);
                lx = x; ly = y;
            }
        } while (x != x1 || y != y1);
    }
    else
    {
        do {
            f += B * signb;
            if (f > 0) {
                f -= A * signa;
                x -= signb;
            }
            y += signa;
            if (abs(lx - x) + abs(ly - y) == 2)
            {
                DrawPixel(lx, y);
                DrawPixel(x, y);
                lx = x; ly = y;
            }
            else
            {
                DrawPixel(x, y);
                lx = x; ly = y;
            }
        } while (x != x1 || y != y1);
    }
}

void sorttreug(int Ax, int Ay, int Bx, int By, int Cx, int Cy)
{
    int tmp;
    int X[3] = { Ay,By,Cy };
    int Y[3] = { Ax,Bx,Cx };
    for (int i = 0; i < 2; i++)
        for (int j = 0; j < 2; j++)
            if (Y[j] < Y[j + 1])
            {
                tmp = Y[j + 1];
                Y[j + 1] = Y[j];
                Y[j] = tmp;
                tmp = X[j + 1];
                X[j + 1] = X[j];
                X[j] = tmp;
            }
    Cy = Y[0]; By = Y[1]; Ay = Y[2];
    Cx = X[0]; Bx = X[1]; Ax = X[2];
    int x1, x2;
    for (int sy = Ay; sy <= Cy; sy++)
    {
        x1 = Ax + (sy - Ay) * (Cx - Ax) / (Cy - Ay);
        if (sy < By) x2 = Ax + (sy - Ay) * (Bx - Ax) / (By - Ay);
        else
        {
            if (Cy == By) x2 = Bx;
            else x2 = Bx + (sy - By) * (Cx - Bx) / (Cy - By);
        }
        if (x1 > x2)
        {
            tmp = x1;
            x1 = x2;
            x2 = tmp;
        }
        DrawLineDDA(sy, x1, sy, x2);
    }
}
void Flood_shape(int x, int y, int fillColor, int boundaryColor) {
    std::stack<std::pair<int, int>> pixelStack;
    pixelStack.push(std::make_pair(x, y));

    while (!pixelStack.empty()) {
        std::pair<int, int> pixel = pixelStack.top();
        pixelStack.pop();

        int currentX = pixel.first;
        int currentY = pixel.second;

        if (GetPixel(hdc, currentX, currentY) != boundaryColor &&
            GetPixel(hdc, currentX, currentY) != fillColor) {

            // Закраска текущего пикселя
            DrawPixel(currentX, currentY, fillColor);

            // Добавление соседних пикселей в стек
            pixelStack.push(std::make_pair(currentX + 1, currentY));
            pixelStack.push(std::make_pair(currentX - 1, currentY));
            pixelStack.push(std::make_pair(currentX, currentY + 1));
            pixelStack.push(std::make_pair(currentX, currentY - 1));
        }
    }
}



void NOTmain(int width, int height) {
    float aspect = (float)width / height;
    float pixelAspect = 1.0f;
    char gradient[] = " .:!/r(l1Z4H9W8$@";
    int gradientSize = std::size(gradient) - 2;

    wchar_t* screen = new wchar_t[width * height];
    //HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    //SetConsoleActiveScreenBuffer(hConsole);
    //DWORD dwBytesWritten = 0;

    int t = 0;
    for (t = 0; t < 10000; t++) {
        vec3 light = norm(vec3(-5, 10, 10));
        vec3 spherePos = vec3(sin(0.1 * t), 0, sqrt(1 - 1 * sin(0.1 * t) * sin(0.1 * t) / 6));
        for (int i = 0; i < width; i++) {
            for (int j = 0; j < height; j++) {

                vec2 uv = vec2(i, j) / vec2(width, height) * 2.0f - 1.0f;
                uv.x *= aspect * pixelAspect;

                vec3 ro = vec3(-6, 0, 0);
                vec3 rd = norm(vec3(2, uv));

                ro = rotateY(ro, 0.25);
                rd = rotateY(rd, 0.25);
                ro = rotateZ(ro, t * 0.01);
                rd = rotateZ(rd, t * 0.01);

                float diff = 1;

                for (int k = 0; k < 5; k++) {
                    float minIt = 100000;

                    vec2 intersection = sphere(ro - spherePos, rd, 1);
                    vec3 n = 0;
                    float albedo = 1;

                    if (intersection.x > 0) {
                        vec3 itPoint = ro - spherePos + rd * intersection.x;
                        minIt = intersection.x;
                        n = norm(itPoint);
                    }


                    /*
                    float bagel = t;
                    intersection = getDist((ro, rd, 0.1), bagel);
                    if (intersection.x > 0 && intersection.x < minIt) {
                        minIt = intersection.x;
                        n = bagel;
                    }
                    */


                    vec3 boxN = 0;
                    intersection = box(ro, rd, 1, boxN);
                    if (intersection.x > 0 && intersection.x < minIt) {
                        minIt = intersection.x;
                        n = boxN;
                    }


                    /*
                    intersection = plane(ro, rd, vec3(0, 0, -1), 1);
                    if (intersection.x > 0 && intersection.x < minIt) {
                        minIt = intersection.x;
                        n = vec3(0, 0, -1);
                        albedo = 0.5;
                    }
                    */
                    if (minIt < 99999) {
                        diff *= (dot(n, light) * 0.5 + 0.5) * albedo;
                        ro = ro + rd * (minIt - 0.01);
                        rd = reflect(rd, n);
                    }
                    else break;
                }

                int color = (int)(diff * 20);
                if (color == 20) color = 0;
                color = clamp(color, 0, gradientSize);
                char pixel = gradient[color];
                int cont = 14;
                if (color != -1) Pixel(i, j, RGB(cont * color, cont * color, cont * color));
                //screen[i + j * width] = pixel;
            }
        }
        screen[width * height - 1] = '\0';
        //WriteConsoleOutputCharacter(hConsole, screen, width * height, { 0, 0 }, &dwBytesWritten);
        //WriteScrin(screen, width, height);
    }
}
 void WriteScrin(wchar_t* screen, int width, int height) {
    char gradient[] = " .:!/r(l1Z4H9W8$@";
    int cont = 14;
    for (int i = 0; i < width; i++) {
        for (int j = 0; j < height; j++) {
            for (int z = 0; z < 18; z++) {
                if (screen[i + j * width] == gradient[z])   Pixel(i, j, RGB(cont * z, cont * z, cont * z));
            }
        }
    }
}