#include<opencv2/imgcodecs.hpp>
#include<opencv2/highgui.hpp>
#include<opencv2/imgproc.hpp>
#include<opencv2/objdetect.hpp>
#include<iostream>

using namespace cv;
using namespace std;

 Mat img;
 VideoCapture cap(0);
vector<vector<int>> newPoints;
vector<vector<int>> myColors {{127,190,69,179,255,255},//orange
                                {22,119,92,49,249,255}};//highlighter yellow

vector<Scalar> myColorsValues {{255,0,255},{0,255,0}};

Point getcontours(Mat imgDia)
{
    /*{{Point(20,30),Point(50,60)},
    {},
    {}}*/
    vector<vector<Point>> contours;
    vector<Vec4i>hierarchy;
    findContours(imgDia,contours,hierarchy,RETR_EXTERNAL,CHAIN_APPROX_SIMPLE);
    //drawContours(img,contours,-1,Scalar(255,0,255),2);
    vector<vector<Point>> conPoly(contours.size());
    vector<Rect> boundRect(contours.size());
    Point myPoint(0, 0);
    //variations
    
    for (int i = 0; i < contours.size(); i++)
    {
        int area=contourArea(contours[i]);
        cout<<area<<endl;
        
        
        string objType;

        if(area>2000)
        {
            float peri=arcLength(contours[i],true);
            approxPolyDP(contours[i],conPoly[i],0.02*peri,true);
            
            cout<<conPoly[i].size()<<endl;
            boundRect[i]=boundingRect(conPoly[i]);
            myPoint.x=boundRect[i].x+boundRect[i].width/2;
            myPoint.y=boundRect[i].y;

           /* int objCor=(int) conPoly[i].size();

            if(objCor==3){objType="Triangle";}
            else if (objCor==4){
                float aspRatio= (float)boundRect[i].width/(float)boundRect[i].height;
                cout<<aspRatio<<endl;
                if(aspRatio>0.95 && aspRatio<1.05){objType="Sqare";}
                else {objType="Rectangle";}
                }
            else if (objCor>4) {objType="Circle";}
            */
            drawContours(img,conPoly,i,Scalar(255,0,255),2);
            rectangle(img,boundRect[i].tl(),boundRect[i].br(),Scalar(0,255,0),5);
            //putText(img,objType,{boundRect[i].x,boundRect[i].y-5},FONT_HERSHEY_DUPLEX,0.25,Scalar(0,69,255),2);
        }
    }
    return myPoint;
}


vector<vector<int>> findColor(Mat img)
{
    Mat imgHSV;
    cvtColor(img,imgHSV,COLOR_BGR2HSV);
    for(int i=0;i<myColors.size();i++)
    {
    Scalar lower(myColors[i][0],myColors[i][1],myColors[i][2]);
    Scalar upper(myColors[i][3],myColors[i][4],myColors[i][5]);
    Mat mask;
    inRange(imgHSV,lower,upper,mask);
    //imshow(to_string(i),mask);
    Point myPoint=getcontours(mask);
    if(myPoint.x !=0 /*&& myPoint.y !=0*/){
    newPoints.push_back({myPoint.x,myPoint.y,i});
    }
    }
    return newPoints;
}

void drawOnCanvas(vector<vector<int>> newPoints,vector<Scalar> myColorsValues)
{
    for(int i=0;i<newPoints.size();i++)
    {
        circle(img,Point(newPoints[i][0],newPoints[i][1]),10,myColorsValues[newPoints[i][2]],FILLED);
    }
}

int main()
{
    
    //Mat img;
    while (true)
    {
        cap.read(img);
        newPoints=findColor(img);
        drawOnCanvas(newPoints,myColorsValues);
        imshow("Image",img); 
        waitKey(1);
    }
    
    return 0;
}