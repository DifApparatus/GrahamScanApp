#include "GrahamScanApp.h"
#include <wx/display.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <limits>

GrahamScanApp::GrahamScanApp(const wxString& title)
       : wxFrame(NULL, wxID_ANY, title, wxDefaultPosition, wxSize())
{
    this->SetSize(horizontalRes,verticalRes);
    wxBoxSizer *vbox = new wxBoxSizer(wxVERTICAL);
    convHullButton = new wxButton(this, wxEVT_BUTTON, "Build Convex Hull");
    convHullButton->Bind(wxEVT_BUTTON, &GrahamScanApp::OnButtonPressed,this);
    vbox->Add(convHullButton);

    char bits[bitmapHor*bitmapVert/8];
    GenerateBitmapField(bits,bitmapHor*bitmapVert, desiredPointQuantity);
    wxBitmap bmp(bits,bitmapHor,bitmapVert);
    sb = new wxStaticBitmap(this,-1,bmp);
    vbox->Add(sb);

    Centre();
    this->SetSizerAndFit(vbox);
}

// Generate bitmap to draw and pixelArray to work with
void GrahamScanApp::GenerateBitmapField(char bits[], int pixelQuantity, int const desiredPointQuantity)
{
    double prob = desiredPointQuantity/static_cast<double>(pixelQuantity);
    srand(time(NULL));
    for (int i=0;i<pixelQuantity/8;i++){
        bits[i] = 0;
        for (int j=0,k=1;k < 256;k*=2,j++) {
            double r = rand() / static_cast<double>(RAND_MAX);
            if (r <= prob) {
                bits[i]+=k;
                int pixelNumber = 8*i+j;
                pointArray.push_back(wxPoint(pixelNumber%bitmapHor, pixelNumber/bitmapHor));
            }
        }
    }
    return;
}

void GrahamScanApp::GrahamScan(wxVector<wxPoint> pointArray)
{
    wxVector<wxPoint> pointQueue(pointArray);
    std::sort(pointQueue.begin() + 1,pointQueue.end(), PointSorterByAngle(pointQueue[0]));
    pointQueue.push_back(pointArray[0]);
    pointQueue.push_back(pointArray[1]);

    convexHull.clear();
    convexHull.push_back(pointQueue[0]);
    convexHull.push_back(pointQueue[1]);
    wxPoint v = pointQueue[2];
    wxPoint w = pointQueue[3];
    int counter = 4;
    while (counter<pointQueue.size()){
        if (IsRightRotation(convexHull.back(), v, w)){
            v = convexHull.back();
            convexHull.pop_back();
        }
        else {
            convexHull.push_back(v);
            v = w;
            w = pointQueue[counter++];
            /*w = pointQueue.front();
            pointQueue.erase(pointQueue.begin());*/
        }
    }
    for (wxPoint &point:convexHull){
        int x = point.x;
        point.x = point.y;
        point.y = x;
    }
}
bool GrahamScanApp::IsRightRotation(wxPoint a, wxPoint b, wxPoint c){
    double *pa = new double[2];
    double *pb = new double[2];
    double *pc = new double[2];
    pa[0] = a.x; pb[0] = b.x; pc[0] = c.x;
    pa[1] = a.y; pb[1] = b.y; pc[1] = c.y;
    double determinant = exact::orient2d(pa,pb,pc);
    return (determinant <= 0) ? true : false;
}


void GrahamScanApp::OnButtonPressed(wxCommandEvent& event)
{
    //because of bitmap specific case we should do this rotation
    for (wxPoint &point:pointArray){
        int x = point.x;
        point.x = point.y;
        point.y = x;
    }
    GrahamScan(pointArray);
    for (wxPoint &point:pointArray){
        int x = point.x;
        point.x = point.y;
        point.y = x;
    }
    wxClientDC dc(sb);
    dc.SetPen(*wxRED_PEN);
    dc.SetBrush( *wxTRANSPARENT_BRUSH);
    wxPoint *hullToDraw = new wxPoint[convexHull.size()];
    int i = 0;
    for (wxPoint point : convexHull){
        hullToDraw[i++] = point;
    }
    wxPoint sbPoint = sb->GetPosition();
    dc.SetLogicalOrigin(-sbPoint.x,-sbPoint.y);
    sb->Refresh();
    sb->Update();
    dc.DrawPolygon(convexHull.size(), hullToDraw);
    return;
}

bool PointSorterByAngle::operator()(wxPoint b, wxPoint c)
{
    double *pa = new double[2];
    double *pb = new double[2];
    double *pc = new double[2];
    pa[0] = basePoint.x; pb[0] = b.x; pc[0] = c.x;
    pa[1] = basePoint.y; pb[1] = b.y; pc[1] = c.y;
    double determinant = exact::orient2d(pa,pb,pc);
    return (determinant > 0);
}
