#include <bits/stdc++.h>
#include <graphics.h>
using namespace std;

int countPoint=0;
int countCircle=0;

// Point structure
class Point{
    public:
    int x,y,pointNo;
    
    

    Point(int i,int j){
        x = i;
        y = j;
        countPoint++;
        pointNo=countPoint;
    }

    
};

// Circle structure
class Circle{
    public:
    float x,y,rad;
    int circleNo,flag=0;
    vector<Point> belongsCircle;
    
    Circle(float k,float l,float m){
        x=k;
        y=l;
        rad=m;
        countCircle++;
        circleNo=countCircle;
    }
    void setBelongPoint(Point p){
        belongsCircle.push_back(p);
    }

    vector<Point> getAllPointBelong(){
        return belongsCircle;
    }

    void setFlag(){
        flag=1;
    }
};

// Check whether the point belongs to circle or not
bool checkPointBelongsCircle(Circle circle,Point point){
    float square_diffX = (circle.x-point.x)*(circle.x-point.x);
    float square_diffY = (circle.y-point.y)*(circle.y-point.y);
    float distance = sqrt(square_diffX + square_diffY);

    return (distance<=circle.rad)?1:0;
}

//Check if the given point is present or not
bool isPresentPrev(Circle circle,Point point){
    for(int i=0;i<circle.belongsCircle.size();i++){
        if(circle.belongsCircle[i].pointNo==point.pointNo)return 0;
    }
    return 1;
}

// Calculate centroid of given points and radius is distance between max(point(i)to centroid) 
Circle getCircle(vector<Point> chosePoint){
    float centerX,centerY,sumX=0,sumY=0;
    int vec_size = chosePoint.size();
    

    for (int i = 0; i < vec_size; i++)
    {
        sumX+=chosePoint[i].x;
        sumY+=chosePoint[i].y;
    }
    centerX = (1.0)*sumX/vec_size;
    centerY = (1.0)*sumY/vec_size;

    float max_rad = 0;
    
    for (int i = 0; i < vec_size; i++)
    {
        float square_diffX = (centerX-chosePoint[i].x)*(centerX-chosePoint[i].x);
        float square_diffY = (centerY-chosePoint[i].y)*(centerY-chosePoint[i].y);
        float distance = sqrt(square_diffX + square_diffY);
        max_rad = max(distance,max_rad);
    }


    Circle cir = Circle(centerX,centerY,max_rad);

    for(int i=0;i<vec_size;i++){
        cir.setBelongPoint(chosePoint[i]);
    }
    
    return cir;
}

bool comp(Point p,Point q){
    return (p.pointNo<q.pointNo)?1:0;
}

bool comp2(Circle p,Circle q){
    return (p.rad<q.rad)?1:0;
}

bool belongCircleSame(vector<Point> belongsCircle1,vector<Point> belongsCircle2){
    for(int i=0;i<belongsCircle1.size();i++){
        if(belongsCircle1[i].pointNo!=belongsCircle2[i].pointNo){
            return 0;
        }
    }

    return 1;
}


int main(int argc,char const *argv[]){

    //Graphics initalization
    int gd= DETECT, gm;
    initgraph(&gd,&gm,(char*)"");
    
    //input
    int n;
    cin>>n;

    vector<Point> pointArray;
    vector<Circle> circleArray;

    for (int i = 0; i < n; i++)
    {
        int x,y;
        cout<<"Point no "<<i+1<<": ";
        cin>>x>>y;
        Point p = Point(x,y);
        pointArray.push_back(p);
        cout<<endl;
    }
    

    // Getting all possible permutation
    int pow_size = pow(2,n);
    for(int i=0;i<pow_size;i++){
        vector<Point> subset;
        
        for(int j=0;j<n;j++){
            if(i & (1<<j)){
                subset.push_back(pointArray[j]);
            }
        }
        
        
        if(subset.size()!=0)
        {
            // Get the circle with minimum possible radius from given set 
            Circle cir = getCircle(subset);
            circleArray.push_back(cir);
           //cout<<"Circle = X: "<<cir.x<<" Y: "<<cir.y<<" Radius: "<<cir.rad<<endl;
        }
        subset.clear();
    }


    // Add the points belongs to the circle other than subset
    for(int i=0;i<circleArray.size();i++){
        for(int j=0;j<pointArray.size();j++){
            if(checkPointBelongsCircle(circleArray[i],pointArray[j]) && isPresentPrev(circleArray[i],pointArray[j])){
                circleArray[i].setBelongPoint(pointArray[j]);
            }
        }
    }

    
    //Sort the belong point  w.r.t Point no.
    for(int i=0;i<circleArray.size();i++){
        sort(circleArray[i].belongsCircle.begin(),circleArray[i].belongsCircle.end(),comp);
    }
    //Sort circles 
    sort(circleArray.begin(),circleArray.end(),comp2);
    

    // Taking circles having less radius and contains same points
    vector<Circle> resCircleArray;
    for(int i=0;i<circleArray.size();i++){
        if(circleArray[i].flag)continue;

        circleArray[i].setFlag();
        Circle minRadius = Circle(circleArray[i].x,circleArray[i].y,circleArray[i].rad);
        minRadius.circleNo = circleArray[i].circleNo;
        for(int j=i+1;j<circleArray.size();j++){
            if(circleArray[i].belongsCircle.size()==circleArray[j].belongsCircle.size() && circleArray[j].flag==0){
                if(belongCircleSame(circleArray[i].belongsCircle,circleArray[j].belongsCircle)){
                    if(minRadius.rad > circleArray[j].rad){
                        minRadius.x =circleArray[j].x;
                        minRadius.y =circleArray[j].y;
                        minRadius.rad =circleArray[j].rad;
                        minRadius.circleNo = circleArray[j].circleNo;
                    }
                    circleArray[j].setFlag();
                    //resCircleArray.push_back();
                }
            }
        }

        for(int k=0;k<circleArray[i].belongsCircle.size();k++){
            minRadius.belongsCircle.push_back(circleArray[i].belongsCircle[k]);
        }
        
        resCircleArray.push_back(minRadius);
    }

    //Display details on terminal
    for(int i=0;i<resCircleArray.size();i++){
        cout<<"Circle "<<i+1<<" => X: "<<resCircleArray[i].x<<" Y: "<<resCircleArray[i].y<<" Radius : "<<resCircleArray[i].rad<<endl;
        cout<<"Contain points => ";
        for(int j=0;j<resCircleArray[i].belongsCircle.size();j++){
            cout<<"Point No:"<<resCircleArray[i].belongsCircle[j].pointNo<<"  ";
        }
        cout<<endl;
    }
    cout<<" Total possible cannonical circles : "<<resCircleArray.size();


    
    // Display details graphically
    for(int i=0;i<resCircleArray.size();i++){
        if(resCircleArray[i].rad==0){
            setfillstyle(SOLID_FILL,YELLOW);                               //Point code
            circle(resCircleArray[i].x,resCircleArray[i].y,6);               //
            floodfill(resCircleArray[i].x,resCircleArray[i].y,15);           //
        }
        else{
            delay(500);
            circle(resCircleArray[i].x,resCircleArray[i].y,resCircleArray[i].rad);// circle code
        }
    }
    getch();
    closegraph();
    
    

    return 0;

}