#ifndef __SHAPE_H__
#define __SHAPE_H__

#include "GameUtil.h"

using namespace std;

typedef list< shared_ptr<GLVector2f> > GLVector2fList;
	
class Shape
{
public:
	Shape();
	Shape(const string& shape_filename);
	virtual ~Shape();
	
	void Render(void);

	void LoadShape(const string& shape_filename);

	const GLVector3f& GetRGBColour() { return mRGB; }
	const GLVector2fList& GetPoints() { return mPoints; } 

	void setRGBColour(float r, float g, float b) {
		mRGB[0] = r;
		mRGB[1] = g;
		mRGB[2] = b;
	}

private:
	bool mLoop;
	GLVector3f mRGB;
	GLVector2fList mPoints;
};

#endif