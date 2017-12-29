#include <iostream>

int fill(int input = 100);     //若参数为100则刷新整张表,否则刷新对应box
int solve(int input = 0);          //若参数为零则刷新整张表,否则刷新对应区域
int isOnly(int i);          //返回唯一值,为零表示不唯一
void clean(int input);
int LTnum(int i);

/*
 * 左上角单元格坐标为[00]，右下角坐标为[88]
 * 09,19,29,...,89均被放弃使用
 *
 */

struct box_struct
{
    int num;
    bool able[10];      //0,1~9
};



box_struct box[108];   //实际使用范围：0～89 (0~107)
box_struct tempbox;

int main()
{
    std::cout << "\n\n\n\n";

    //输入+反馈
    {
        using namespace std;
        cout << "Input the sudoku numbers.\n"
                "Input 0 for unknown grids.\n"
                "Please input in only ONE line:\n";


        char tempch;
        for(int i = 0; i <= 89; i++)
        {
            //跳过下标为[X9]的数组
            if(i % 10 == 9)
                continue;

            //忽略输入区的空格和换行
            do
                cin.get(tempch);
            while(tempch == ' ' || tempch == '\n');

            box[i].num = int(tempch) - int('0');
        }


        cout << "\nHere are what you inputed:\n";
        for(int i = 0; i <= 89; i++)
        {
            //跳过下标为[X9]的数组,当输出完3个数字输出空格,输出完9个数字换行
            if(i % 10 == 9)
                continue;

            cout << box[i].num;

            if(i % 10 == 2 || i % 10 == 5)
                cout << "   ";
            if(i % 10 == 8)
                cout << endl;
            if(i == 28 || i == 58)
                cout << endl;
        }

    }

    fill(100);


    std::cout << "\n\nResults:\n\n";
    for(int i = 0; i <= 89; i++)
    {
        using namespace std;
        //跳过下标为[X9]的数组,当输出完3个数字输出空格,输出完9个数字换行
        if(i % 10 == 9)
            continue;

        cout << box[i].num;

        if(i % 10 == 2 || i % 10 == 5)
            cout << "   ";
        if(i % 10 == 8)
            cout << endl;
        if(i == 28 || i == 58)
            cout << endl;
    }

    return 0;
}

int fill(int input)
{
    int start, end;

    //确定是单个数据还是全表
    if(input == 100)
    {
        start = 0;
        end = 89;
    }
    else
    {
        start = input;
        end = input;
    }


    int change = 0;
    for(int i = start; i <= end; i++)
    {
        //跳过已知数据
        if(box[i].num != 0)
            continue;

        //清零
        clean(i);

        //将对应.able设为true
        for(int l = 0; l <= 89; l++)
        {

            //跳过下标为[X9]的数组
            if(l % 10 == 9)
                continue;

            //检索同行/同列/同区块的数据
            if(l % 10 == i % 10 ||
                    l / 10 == i / 10 ||
                    LTnum(l) == LTnum(i))
            {
                box[i].able[box[l].num] = false;
            }
        }

        if(isOnly(i) != 0)
            change++;

    }
    return change;
}

int solve(int input)
{
    if(input != 0)
        return 0;

    int change = 0;

//    //清零
//    clean(input);

    //制作数组循环
    int n[10] = {};         //实际使用范围0,1~9
    for(int i = 0; i <= 26; i++)
    {
        //制作下标数组
        if(true)
        {
        //行
        if(i >= 0 && i <= 8)
        {
            for(int ni = 1; ni <= 9; ni++)
                n[ni] = i * 10 + (ni - 1);
        }

        //列
        else if(i >= 9 && i <= 17)
        {
            for(int ni = 1; ni <= 9; ni++)
                n[ni] = (ni - 1) * 10 + (i - 9);
        }

        //区块
        else if(i >= 18 && i <= 26)
        {
            //找出start值
            int start = 0;
            int i2 = i - 18;
            switch(i2)
            {
            case 0: start = 00; break;
            case 1: start = 03; break;
            case 2: start = 06; break;
            case 3: start = 30; break;
            case 4: start = 33; break;
            case 5: start = 36; break;
            case 6: start = 60; break;
            case 7: start = 63; break;
            case 8: start = 66; break;
            }

            //赋值给n[1]~n[9]
            if(true)
            {
                n[1] = start + 0;
                n[2] = start + 1;
                n[3] = start + 2;
                n[4] = start + 10;
                n[5] = start + 11;
                n[6] = start + 12;
                n[7] = start + 20;
                n[8] = start + 21;
                n[9] = start + 22;
            }
        }
        }

        //清空,统计
        for(int cleani = 1; cleani <= 9; cleani++)
            tempbox.able[cleani] = true;
        for(int ni = 1; ni <= 9; ni++)
        {
            tempbox.able[
                    box[
                    n[ni]
                    ].num
                    ] = false;
        }

        //返回统计结果至原数据
        for(int ni = 1; ni <= 9; ni++)
        {
            //跳过已知数据
            if(box[n[ni]].num != 0)
                continue;

            for(int ablei = 1; ablei <= 9; ablei++)
            {
                if(tempbox.able[ablei] == false)
                    box[n[ni]].able[ablei] = false;
            }
        }

        //确定结果
        for(int ni = 1; ni <= 9; ni++)
        {
            //跳过已知数据
            if(box[n[ni]].num != 0)
                continue;

            if(isOnly(n[ni]))
                change++;
        }
    }
    return change;
}

int isOnly(int i)
{
        int possible = 9;
        for(int l = 1; l <= 9; l++)
        {
            if(!box[i].able[l])
                possible--;
        }

        //以下这段程序运行于:该box可以确定数字.这段程序目的是找出那个数字
        if(possible == 1)
        {
            for(int l = 1; l <= 9; l++)
            {
                if(box[i].able[l])
                    box[i].num = l;
            }
            clean(i);
            box[i].able[box[i].num] = true;
        }
    return box[i].num;
}

void clean(int input)
{
    for(int i = 1; i <= 9; i++)
        box[input].able[i] = false;
}

int LTnum(int i)
{
    return ((i - i % 10) - (i - i % 10) % 30) +
            ((i % 10)     - (i % 10)     % 3 );
}

