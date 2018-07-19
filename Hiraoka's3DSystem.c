//
//  hiraocar.c
//
//
//  Created by raoka0000 on 2015/09/30.
//
//

#include <stdio.h>
#include <handy.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>

#define RADIUS 5
#define RAD_CAMERA 10
#define SPEED 1
#define SPEED2 0.2
#define WIDTH 500
#define HEIGHT 500
#define OUTLOOK 5*M_PI/18
#define MAX 100

typedef struct Vec3{
    double x;
    double y;
    double z;
} Vec3;

typedef struct Vec2{
    double x;
    double y;
} Vec2;

typedef struct Camu{
    double x;
    double y;
    double z;
    double bases[3][3];//カメラの向きの基底
} Camu;

typedef struct triangle{
    int p1;
    int p2;
    int p3;
    int color;
} triangle;

typedef struct solid{
    Vec3 *coo;
    triangle *surface;
    int total_Vec3;
    int total_surface;
} solid;
typedef struct DrawOrder{//クイックソート用構造体
    double sam_distance;
    int hash;
}DrawOrder;

/*グローバル変数*/
Camu _MinCamera = {0,0,-10,{{1,0,0},{0,1,0},{0,0,1}}};//メインカメラの情報
Camu _MinCamera2 = {0,0,-10,{{1,0,0},{0,1,0},{0,0,1}}};//メインカメラの情報
Vec2 _ScreenCenter = {250,250};//消失点の座標
solid World;//すべての立体を統合した存在。
int teble_cun = 0;
solid teble[MAX];//立体情報をここでまとめる。

/*プロトタイプ宣言*/
Vec2 keisan(Vec3 coo,Camu camu);

solid SolidCreate(char *file_neme);
Vec3 Rotmatrix(Vec3 bases,double agr);
void Draw(solid Solid,int lid,Camu camu);
int DrawOrdercomp( const void *c1, const void *c2 );
void GetMultiplyMatrix(double a[3][3],double b[3][3],double Rev[3][3]);
void GetInverseMatrix(double a[3][3],double Rev[3][3]);
Vec3 GetTransformationMatrix(double b[3][3],Vec3 a);
void Integration(solid child);

void addChild(solid a){
    if (teble_cun < MAX) {
        teble[teble_cun] = a;
        teble_cun += 1;
    }else{
        printf("これ以上立体データを作れません\n");
    }
}

void TebleInteg(){
    int i;
    World = SolidCreate("DummyData.txt");
    for (i = 0; i<teble_cun; i++) {
        Integration(teble[i]);
    }
}

void Integration(solid child){
    int i;
    solid Rev = World;
    Rev.total_Vec3 += child.total_Vec3;
    Rev.total_surface += child.total_surface;
    Rev.coo = (Vec3*) realloc(World.coo,Rev.total_Vec3*sizeof(Vec3));
    Rev.surface =(triangle*) realloc(World.surface,Rev.total_surface*sizeof(triangle));

    //ファイルの読み込み。
    for (i = 0; i< child.total_Vec3;i++) {
        Rev.coo[i + World.total_Vec3] = child.coo[i];
    }
    for (i = 0; i< child.total_surface;i++) {
        Rev.surface[i + World.total_surface].p1 = child.surface[i].p1 + World.total_Vec3;
        Rev.surface[i + World.total_surface].p2 = child.surface[i].p2 + World.total_Vec3;
        Rev.surface[i + World.total_surface].p3 = child.surface[i].p3 + World.total_Vec3;
        Rev.surface[i + World.total_surface].color = child.surface[i].color;
    }
    World = Rev;
}

int main(void){

    double Theta = 3*M_PI/180;
    double Up[3][3] = {{1,0,0},{0,cos(Theta),-1*sin(Theta)},{0,sin(Theta),cos(Theta)}};
    double Down[3][3] = {{1,0,0},{0,cos(-Theta),-1*sin(-Theta)},{0,sin(-Theta),cos(-Theta)}};
    double Right[3][3] = {{cos(Theta),0,-1*sin(Theta)},{0,1,0},{sin(Theta),0,cos(Theta)}};
    double Left[3][3] = {{cos(-Theta),0,-1*sin(-Theta)},{0,1,0},{sin(-Theta),0,cos(-Theta)}};

    int flg = -1;
    doubleLayer layers_main,layers_sub; /* (1) ダブルレイヤの変数を宣言する */
    int win_bace = HgWOpen(100, 200, WIDTH, HEIGHT);//ウィインドサイズ
    int win_sub = HgWOpen(WIDTH + 120, 200, WIDTH, HEIGHT);
    layers_main = HgWAddDoubleLayer(win_bace); /* (2) ダブルレイヤを作成する */
    layers_sub = HgWAddDoubleLayer(win_sub);
    World = SolidCreate("DummyData.txt");
    solid cube1 = SolidCreate("data2-3.txt");
    addChild(cube1);
    addChild(SolidCreate("data2-2.txt"));
    addChild(SolidCreate("data2-1.txt"));

    for(;;){
        HgSleep(0.01);
        int lid_main = HgLSwitch(&layers_main); /* (3) レイヤを切り替える */
        int lid_sub = HgLSwitch(&layers_sub);
        HgLClear(lid_main); /* レイヤを消去 */
        HgLClear(lid_sub);
        hgevent *event;  // HandyGraphicのイベントを扱うための変数
        HgSetEventMask(HG_KEY_DOWN|HG_MOUSE_DOWN);  // イベント発生処理
        event = HgEventNonBlocking();

        if(event != NULL && event->type == HG_KEY_DOWN){
            if (event->ch ==  0x71) {
                HgClose();
                return 0;
            }
            switch (event->ch) {
                    //第一画面
                case HG_R_ARROW://右 1
                    _MinCamera.x += SPEED;
                    break;
                case HG_L_ARROW://左 2
                    _MinCamera.x -= SPEED;
                    break;
                case HG_U_ARROW://奥 3
                    _MinCamera.z += SPEED;
                    break;
                case HG_D_ARROW://手前 4
                    _MinCamera.z -= SPEED;
                    break;
                case 'z'://上 5
                    _MinCamera.y += SPEED;
                    break;
                case 'x'://下 6
                    _MinCamera.y -= SPEED;
                    break;
                case 'w':
                    GetMultiplyMatrix(Up,_MinCamera.bases,_MinCamera.bases);
                    break;
                case 's':
                    GetMultiplyMatrix(Down,_MinCamera.bases,_MinCamera.bases);
                    break;
                case 'd':
                    GetMultiplyMatrix(Right,_MinCamera.bases,_MinCamera.bases);
                    break;
                case 'a':
                    GetMultiplyMatrix(Left,_MinCamera.bases,_MinCamera.bases);
                    break;
                    //第二画面
                case '1'://右 1
                    _MinCamera2.x += SPEED;
                    break;
                case '2'://左 2
                    _MinCamera2.x -= SPEED;
                    break;
                case '3'://奥 3
                    _MinCamera2.z += SPEED;
                    break;
                case '4'://手前 4
                    _MinCamera2.z -= SPEED;
                    break;
                case '5'://上 5
                    _MinCamera2.y += SPEED;
                    break;
                case '6'://下 6
                    _MinCamera2.y -= SPEED;
                    break;
                case '7':
                    GetMultiplyMatrix(Up,_MinCamera2.bases,_MinCamera2.bases);
                    break;
                case '8':
                    GetMultiplyMatrix(Down,_MinCamera2.bases,_MinCamera2.bases);
                    break;
                case '9':
                    GetMultiplyMatrix(Right,_MinCamera2.bases,_MinCamera2.bases);
                    break;
                case '0':
                    GetMultiplyMatrix(Left,_MinCamera2.bases,_MinCamera2.bases);
                    break;
                case 'c':
                    flg = (flg < teble_cun)? flg+1:0;
                    break;
                case 'v':
                {char *say = "say hiraoka 3d system";
                    system(say);}
                    break;
                default:
                    break;
            }
        }
        //雑にアニメーションの追加
        if (flg != -1) {
            for (int i = 0; i<teble[flg].total_Vec3; i++) {
                teble[flg].coo[i] = GetTransformationMatrix(Left,teble[flg].coo[i]);
            }
        }
        Draw(World,lid_main,_MinCamera);
        Draw(World,lid_sub,_MinCamera2);
    }
    return 0;
}


Vec2 keisan(Vec3 coo,Camu camu){
    double pers,r,theta,phicos,phisin;
    Vec3 distance = {coo.x-camu.x,coo.y-camu.y,coo.z-camu.z};
    double rate_x,rate_y;
    double tmp;
    Vec2 Rev = {0,0};
    double inverse[3][3];
    //正と負で場合分け
    distance = GetTransformationMatrix(camu.bases,distance);
    if (distance.z <= 0) {
        return Rev;
    }
    if (distance.x==0) {
        Rev.x = _ScreenCenter.x;
    }
    if (distance.x>0) {
        tmp = atan2(distance.x,distance.z);
        rate_x = tmp/(OUTLOOK);
        Rev.x = _ScreenCenter.x + rate_x*WIDTH;
    }
    if (distance.x<0) {
        tmp = atan2(distance.x * -1,distance.z);
        rate_x = tmp/(OUTLOOK);
        Rev.x = _ScreenCenter.y - rate_x*WIDTH;
    }
    if (distance.y==0) {
        Rev.y = _ScreenCenter.y;
    }
    if (distance.y>0) {
        tmp = atan2(distance.y,distance.z);
        rate_y = tmp/(OUTLOOK);
        Rev.y = _ScreenCenter.y + rate_y*HEIGHT;
    }
    if (distance.y<0) {
        tmp = atan2(distance.y * -1,distance.z);
        rate_y = tmp/(OUTLOOK);
        Rev.y = _ScreenCenter.y - rate_y*HEIGHT;
    }
    return Rev;
}


void Draw(solid Solid,int lid,Camu camu){
    TebleInteg();
    int i;
    double tmp;
    double distance[Solid.total_Vec3];
    DrawOrder order[Solid.total_surface];
    //カメラと各点の距離を測る
    for (i = 0; i < Solid.total_Vec3 ; i++) {
        distance[i] = pow(((Solid.coo + i) ->x) -camu.x  ,2)
        + pow(((Solid.coo + i) ->y) - camu.y  ,2)
        + pow(((Solid.coo + i) ->z )- camu.z  ,2);
    }
    for (i = 0; i < Solid.total_surface; i++) {
        order[i].sam_distance = distance[(Solid.surface + i)-> p1] + distance[(Solid.surface + i)-> p2] + distance[(Solid.surface + i)-> p3];
        order[i].hash = i;
        //printf("%f　ハッシュ:%d\n",order[i].sam_distance,order[i].hash);
    }
    qsort(order, Solid.total_surface, sizeof(DrawOrder), DrawOrdercomp );//構造体を基準にソート
    //描写
    for (i = 0; i < Solid.total_surface; i++) {
        Vec2 Dis;
        double x[3];
        double y[3];
        int tmp[4];
        int j,flg;
        tmp[0] = order[i].hash;
        tmp[1] = (Solid.surface + tmp[0])-> p1;
        tmp[2] = (Solid.surface + tmp[0])-> p2;
        tmp[3] = (Solid.surface + tmp[0])-> p3;
        for (j = 0; j < 3; j++) {
            Dis = keisan(*(Solid.coo + tmp[j+1]),camu);
            x[j] = Dis.x;
            y[j] = Dis.y;
        }
        HgWSetFillColor(lid,(Solid.surface + tmp[0])-> color);
        HgWSetColor(lid,(Solid.surface + tmp[0])-> color);
        HgWPolygonFill(lid,3, x, y, 1);//描写
    }
}
solid SolidCreate(char *file_neme){
    int i,cun;//汎用関数
    int total_Vec3,total_surface;
    solid Rev;
    FILE *fp;//ファイルの宣言
    /*ファイルから３Dデータの読み込み*/
    fp = fopen(file_neme,"r");
    if(fp == NULL){
        printf("ファイルを開けませんでした\n");
    }
    fscanf(fp,"%d",&total_Vec3);//ファイルから取得する数を取得
    Vec3 coo[total_Vec3];

    Rev.coo = (Vec3*) malloc(total_Vec3*sizeof(Vec3));

    //ファイルの読み込み。
    for (i = 0; i< total_Vec3;i++) {
        fscanf(fp,"%lf,%lf,%lf",&Rev.coo[i].x,&Rev.coo[i].y,&Rev.coo[i].z);
    }
    fscanf(fp,"%d",&total_surface);//ファイルから取得する数を取得
    triangle surface[total_surface];

    Rev.surface =(triangle*) malloc(total_surface*sizeof(triangle));

    //ファイルの読み込み。
    for (i = 0; i< total_surface;i++) {
        fscanf(fp,"%d,%d,%d,%d",&Rev.surface[i].p1,&Rev.surface[i].p2,&Rev.surface[i].p3,&Rev.surface[i].color);
    }
    fclose(fp);//ファイルを閉じる
    Rev.total_Vec3 = total_Vec3;
    Rev.total_surface = total_surface;
    return Rev;
}
/* クイックソート用比較関数 */
int DrawOrdercomp( const void *c1, const void *c2 )
{
    DrawOrder test1 = *(DrawOrder *)c1;
    DrawOrder test2 = *(DrawOrder *)c2;
    int tmp1 = test1.sam_distance;
    int tmp2 = test2.sam_distance;
    return  tmp2 - tmp1;
}
/*3*3の行列の計算式*/
void GetMultiplyMatrix(double a[3][3],double b[3][3],double Rev[3][3]){
    int i,j,k;
    double c[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
    //受け取った２つの行列の掛け算を行う。
    for(i=0;i<3;i++) {
        for(j=0;j<3;j++) {
            for(k=0;k<3;k++) {
                c[i][j]+=a[i][k]*b[k][j];
            }
        }
    }
    //リターン
    for (i=0; i<3; i++) {
        for (j=0; j<3;j++) {
            Rev[i][j] = c[i][j];
        }
    }
}
/*座標の行列変換*/
Vec3 GetTransformationMatrix(double b[3][3],Vec3 a){
    double c[3] = {0,0,0};//一時的に保存する。
    Vec3 Rev;
    int i,j;
    for (i = 0; i < 3; i++) {
        c[i] += b[i][0] * a.x;
        c[i] += b[i][1] * a.y;
        c[i] += b[i][2] * a.z;
    }
    Rev.x = c[0];
    Rev.y = c[1];
    Rev.z = c[2];
    return Rev;
}
