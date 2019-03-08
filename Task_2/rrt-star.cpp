#include <bits/stdc++.h>
#include "stdio.h"
#include <stdlib.h>
#include <vector>
#include <math.h>
#include "opencv2/core/core_c.h"
#include "opencv2/core/core.hpp"
#include "opencv2/flann/miniflann.hpp"
#include "opencv2/imgproc/imgproc_c.h"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/video/video.hpp"
#include "opencv2/features2d/features2d.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/ml/ml.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "opencv2/highgui/highgui.hpp"
//#include "opencv2/contrib/contrib.hpp"

using namespace cv;
using namespace std;

#define pb push_back
#define START 6
#define END 495
typedef struct
{
  int x;
  int y;
}point;

typedef struct Node1 {
    int parent;
    point position;
    float dist;
}Node1;

typedef struct edge{
	Node1 p1;
	Node1 p2;
}edge;

Node1 start_node;
Node1 end_node;
Mat img;
Node1 nodes[5000];
int totnodes = 0;
int checkdest = 0;
int step_size = 10;
int iter = 0;
int nbd=30.0;
vector<point> fpoints;
//vector<edge> edges;


void init()
{
    start_node.position.x = START;
    start_node.position.y = START;
    start_node.parent = -1;
    start_node.dist=0;
    for(int i=start_node.position.x - 5; i < start_node.position.x + 5&&i>=0; i++)
    {
      for(int j=start_node.position.y - 5; j < start_node.position.y + 5&&j>=0; j++)
      {
        img.at<Vec3b>(i, j)[0] = 255;
        img.at<Vec3b>(i, j)[1] = 0;
        img.at<Vec3b>(i, j)[2] = 0;
      }
    }
    nodes[totnodes++] = start_node;
    end_node.position.x = END;
    end_node.position.y = END;
    //end_node.dist=node_dist(start_node.position,end_node.position);
    for(int i=end_node.position.x - 5; i < end_node.position.x + 5&&i<=img.rows; i++)
    {
      for(int j=end_node.position.y - 5; j < end_node.position.y + 5&&j<=img.cols; j++)
      {
        img.at<Vec3b>(i, j)[0] = 0;
        img.at<Vec3b>(i, j)[1] = 0;
        img.at<Vec3b>(i, j)[2] = 255;
      }
    }
    srand(time(NULL));
}
float node_dist(point p, point q)
{
  point v;
  v.x = p.x - q.x;
  v.y = p.y - q.y;
  return sqrt(pow(v.x, 2) + pow(v.y, 2));
}
int near_node(Node1 rnode)
{
  float min_dist = 10000.0, dist= node_dist(start_node.position, rnode.position);
  int lnode = 0, i = 0;
  for(i=0; i<totnodes; i++)
  {
    dist = node_dist(nodes[i].position, rnode.position);
    if(dist<min_dist)
    {
      min_dist = dist;
      lnode = i;
    }
  }
  return lnode;
}
point stepping(point nnode,point rnode)
{
  point interm, step;
  float magn = 0.0, x = 0.0, y = 0.0;
  interm.x = rnode.x - nnode.x;
  interm.y = rnode.y - nnode.y;
  magn = sqrt((interm.x)*(interm.x) + (interm.y)*(interm.y));
  x = (float)(interm.x / magn);
  y = (float)(interm.y / magn);
  step.x = (int)(nnode.x + step_size*x);
  step.y = (int)(nnode.y + step_size*y);
  return step;
}

int choose_parent(Node1 nnode,Node1 nearnode,int ind)
{	
	
	nnode.dist=nearnode.dist+node_dist(nearnode.position,nnode.position);

	
	for(int i=0;i<totnodes;i++)
	{
		if(node_dist(nodes[i].position,nnode.position)<nbd)
		{
			if(nodes[i].dist+node_dist(nodes[i].position,nnode.position)<=nnode.dist)
			{
				
				//nnode.dist=nodes[i].dist+node_dist(nodes[i].position,nnode.position);
			    ind=i;
			    //cout<<" 1";
			    
			}

		}
	}



	return ind;
	
}





int check_validity_1(point p, point q)
{
  point large, small;
  int i = 0, j1 = 0, j2 = 0;
  double slope;
  if(q.x<p.x)https://www.codechef.com/users/viplaw
  {
    small = q;
    large = p;
  }
  else
  {
    small = p;
    large = q;
  }
  if(large.x == small.x)
    return 0;
  slope = ((double)large.y - small.y)/((double)large.x - small.x);
  for(i=small.x+1; i<large.x; i++)
  {
    j1 = (int)((i*slope) - (small.x)*(slope) + small.y);
    j2 = j1 + 1;
    if((i<0) || (i>550) || (j1<0) || (j1>550) || (j2<0) || (j2>550))
      continue;
    if(((int)img.at<Vec3b>(i, j1)[0] <250) && ((int)img.at<Vec3b>(i, j1)[1] < 250) && ((int)img.at<Vec3b>(i, j1)[2] < 250))
     return 0;
     if(((int)img.at<Vec3b>(i, j2)[0] <250) && ((int)img.at<Vec3b>(i, j2)[1] < 250) && ((int)img.at<Vec3b>(i, j2)[2] < 250))
      return 0;  }
  return 1;
}
int check_validity_2(point p, point q)
{
  point large, small;
  int i = 0, j1 = 0, j2 = 0;
  double slope;
  if(q.y<p.y)
  {
    small = q;
    large = p;
  }
  else
  {
    small = p;
    large = q;
  }
  if(large.x == small.x)
    return 0;
  slope = ((double)large.y - small.y)/((double)large.x - small.x);
  for(i=small.y+1; i<large.y; i++)
  {
    j1 = (int)(((i - small.y)/slope) + small.x);
    j2 = j1 + 1;
    if((i<0) || (i>550) || (j1<0) || (j1>550) || (j2<0) || (j2>550))
      continue;
    if(((int)img.at<Vec3b>(j1, i)[0] <250) && ((int)img.at<Vec3b>(j1, i)[1] < 250) && ((int)img.at<Vec3b>(j1, i)[2] < 250))
     return 0;
     if(((int)img.at<Vec3b>(j2, i)[0] <250) && ((int)img.at<Vec3b>(j2, i)[1] < 250) && ((int)img.at<Vec3b>(j2, i)[2] < 250))
      return 0;
  }
  return 1;
}

void rewire(Node1 n1)
{
	for(int i=0;i<totnodes;i++)
	{					line(img, Point((nodes[i].position).y, (nodes[i].position).x), Point(nodes[nodes[i].parent].position.y, nodes[nodes[i].parent].position.x), Scalar(0, 255, 0), 2, 6,0);

		if(node_dist(nodes[i].position,n1.position)<nbd)
		{	
			if(n1.dist+node_dist(n1.position,nodes[i].position)<nodes[i].dist)
			{	
				//cout<<2<<endl;
				line(img, Point((nodes[i].position).y, (nodes[i].position).x), Point(nodes[nodes[i].parent].position.y, nodes[nodes[i].parent].position.x), Scalar(255, 255, 255), 2, 6,0);
				nodes[i].parent=totnodes-1;
				nodes[i].dist=n1.dist+node_dist(n1.position,nodes[i].position);
				line(img, Point((nodes[i].position).y, (nodes[i].position).x), Point(nodes[nodes[i].parent].position.y, nodes[nodes[i].parent].position.x), Scalar(0, 255, 0), 2, 6,0);

			}
		}
	}
}



void draw_path()
{
  int downind = totnodes-1;
  int upind = totnodes-2;
  ofstream myfile;
  myfile.open("points.txt");
  vector<point> coordi;
  
  while(1)
  {
    line(img, Point((nodes[upind].position).y, (nodes[upind].position).x), Point(nodes[downind].position.y, nodes[downind].position.x), Scalar(255, 255, 0), 2, 6,0);
    cout<<nodes[upind].position.y<<" "<<(nodes[upind].position).x<<endl;
    coordi.pb(nodes[upind].position);
    point temp;
    temp.y=(nodes[upind].position).y;
    temp.x=(nodes[upind].position).x;
    fpoints.push_back(temp);
    if(nodes[upind].parent==-1)
      break;
    downind=upind;
    upind=nodes[upind].parent;
     imshow("window", img);
     waitKey(1);
    

  }

  for(int i=coordi.size()-1;i>0;i--)
  {	
  	if(coordi[i].x==coordi[i-1].x)
  		coordi[i-1].x-=1;
  	if(coordi[i].y==coordi[i-1].y)
  		coordi[i-1].y-=1;
  	myfile<<coordi[i].y<<'\n'<<(img.rows-coordi[i].x)<<'\n';
  }


  point temp1={0,0};
  fpoints.pb(temp1);	
  float sum=0.0;
  vector<float> theta1;
  theta1.pb(0);
  int m=0;
  for(int i=fpoints.size()-1;i>=1;i--)
  {
  	 //myfile<<(float)fpoints[i].y/51.2<<'\n'<<(float)(fpoints[i]).x/51.2<<'\n';
  	float theta;
  	if(fpoints[i].y==fpoints[i-1].y )
  	 theta =1.57;
  else
  	theta =atan(float(fpoints[i].x-fpoints[i-1].x)/(float)(fpoints[i].y-fpoints[i-1].y));

  	theta1.pb(theta);
  	//myfile<<(node_dist(fpoints[i],fpoints[i-1])/512.0)*11.0<<"\n"<<theta1[m+1]-theta1[m]<<"\n";
  	sum+=theta1[m+1]-theta1[m];
  	m++;
  }

  cout<<"sum is "<<sum<<'\n';
  
  myfile.close();
}

void rrt()
{
  int flag1 = 0, index = 0, flag2 = 0;
    Node1 rnode;
    Node1 newn;
    (rnode.position).x = rand() % 512 + 1;
    (rnode.position).y = rand() % 512 + 1;
    index = near_node(rnode);
    if((node_dist(rnode.position, nodes[index].position))<step_size)
      return;
    else
      newn.position = stepping(nodes[index].position,rnode.position);
    flag1 = check_validity_1(nodes[index].position, newn.position);
    flag2 = check_validity_2(nodes[index].position, newn.position);
    if((flag1 == 1) && (flag2 == 1))
    {
      
      newn.parent =choose_parent(newn,nodes[index],index);

      //cout<<newn.parent<<endl;
      
      newn.dist=nodes[newn.parent].dist+node_dist(nodes[newn.parent].position,newn.position);
      nodes[totnodes++] = newn;
      //cout<<node_dist(nodes[newn.parent].position,newn.position)<<" "<< nodes[newn.parent].dist<<endl;
      
     rewire(newn);
      //edges.push_back({newn,nodes[newn.parent]});
      //line(img, Point((newn.position).y, (newn.position).x), Point(nodes[index].position.y, nodes[index].position.x), Scalar(0, 255, 255), 2, 8);
      for(int i=newn.position.x - 2; i < newn.position.x + 2; i++)
      {
        for(int j=newn.position.y-2;j < newn.position.y + 2; j++)
        {
          if((i<0) || (i>512) || (j<0) || (j>512))
            continue;

          img.at<Vec3b>(i, j)[0] = 0;
          img.at<Vec3b>(i, j)[1] = 255;
          img.at<Vec3b>(i, j)[2] = 0;
        }
      }
      if((check_validity_1(newn.position, end_node.position)) && (check_validity_2(newn.position, end_node.position)) && (node_dist(newn.position,end_node.position) < step_size))
      {
        checkdest = 1;
        nodes[totnodes++] = end_node;
        end_node.parent = totnodes-2;
       draw_path();
      }
    }
    iter++;
    imshow("window", img);
    waitKey(1);
}

int main()
{
  img = imread("task2.png", CV_LOAD_IMAGE_COLOR);
  for(int i=0;i<img.rows;i++)
  {
  	for(int j=0;j<img.cols;j++)
  	{
  		if(img.at<Vec3b>(i, j)[0]==0&& img.at<Vec3b>(i, j)[1]==0&&img.at<Vec3b>(i, j)[2]==0)
  		{
  			img.at<Vec3b>(i, j)[0]=255;
  			img.at<Vec3b>(i, j)[1]=255;
  			img.at<Vec3b>(i, j)[2]=255;
  		}

  		else if(img.at<Vec3b>(i, j)[0] ==255&&img.at<Vec3b>(i, j)[1]==255&&img.at<Vec3b>(i, j)[2]==255)
  		{
  			img.at<Vec3b>(i, j)[0]=0;
  			img.at<Vec3b>(i, j)[1]=0;
  			img.at<Vec3b>(i, j)[2]=0;
  		}
  	}
  }
  

  init();
  while((checkdest == 0))
    rrt();

  namedWindow( "window", WINDOW_AUTOSIZE );
  imshow("window", img);
  waitKey();
  return 0;
}
