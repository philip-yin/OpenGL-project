#include "WorkingScene.h"

// This file includes the basic functions that your program must fill in.  
// Note that there are several helper functions from Curve.cpp that can be used!
// In particular, take a look at moveActivePoint, connectTheDots, drawLine, etc.

// What happens when you drag the mouse to x and y?  
// In essence, you are dragging control points on the curve.
void WorkingScene::drag(int x, int y) {
	/* YOUR CODE HERE */
	//you must figure out how to transform x and y so they make sense
	//update oldx, and oldy
	//make sure scene gets redrawn
	float fx = (x - oldx) / float(width);
	float fy = (oldy - y) / float(height);
	theOnlyCurve->moveActivePoint(fx, fy);
	glutPostRedisplay();
	oldx = x;
	oldy = y;
}

// Mouse motion.  You need to respond to left clicks (to add points on curve) 
// and right clicks (to delete points on curve) 
void WorkingScene::mouse(int button, int state, int x, int y) {
	// theOnlyCurve is the current type of curve being drawn. It is created in Scene.cpp.
	if (theOnlyCurve && state == GLUT_DOWN) {
		float fx = x / float(width);
		float fy = (height - y) / float(height);
		if (button == GLUT_LEFT_BUTTON) {
			/* YOUR CODE HERE */
			theOnlyCurve->addPoint(fx, fy);
		}
		if (button == GLUT_RIGHT_BUTTON) {	
			/* YOUR CODE HERE */
			theOnlyCurve->updateActivePoint(fx, fy);
			theOnlyCurve->deleteActivePoint();
		}
	}
	/* YOUR CODE HERE */
	//update oldx, and oldy
	//make sure scene gets redrawn
	glutPostRedisplay();
	oldx = x;
	oldy = y;
}



#include "Bezier.h"

// Bezier drawing function.  This is by deCasteljau or equivalent algorithm. 
// It should support Bezier curves of arbitrary degree/order.
void Bezier::draw(int levelOfDetail) {

	connectTheDots();
	int i, j, k;
	Pvector::iterator p;
	/* YOUR CODE HERE */

	// i is number of control points
	int num = 0;
	for (p = points.begin(); p != points.end(); p++)
		num++;
	
	float u = 1 / float(levelOfDetail);
	
	Pvector temp;
	Point last = Point(0.0, 0.0);

	for (k = 0; num > 1 && k < levelOfDetail; k++) {
		temp = points;
		for (i = num; i > 0; i--) {
			if (i == num)
				continue;
			for (j = 0; j < i; j++) {
				temp.at(j).x = temp.at(j).x * (1-u*(k+1)) + temp.at(j + 1).x *u*(k+1);
				temp.at(j).y = temp.at(j).y * (1-u*(k+1)) + temp.at(j + 1).y *u*(k+1);
			}
		}
		if (k == 0)
			drawLine(points.at(0).x, points.at(0).y, temp.at(0).x, temp.at(0).y);
		else 
			drawLine(last.x, last.y, temp.at(0).x, temp.at(0).y);

		last = temp.at(0);
	}
}



#include "Bspline.h"

// The B-Spline drawing routine.  
// Remember to call drawSegment (auxiliary function) for each set of 4 points.
void Bspline::draw(int levelOfDetail) {
	connectTheDots();
	/* YOUR CODE HERE */
	if (points.size() < 4)
		return;
	Pvector::iterator iter = points.begin();
	while (iter	+3 < points.end() )
	{
		drawSegment(iter, iter + 1, iter + 2, iter + 3, levelOfDetail);
		iter++;
	}
}

void Bspline::drawSegment(Pvector::iterator p1, Pvector::iterator p2, Pvector::iterator p3, Pvector::iterator p4, int levelOfDetail) {
	float x, y;

	/* YOUR CODE HERE */
	float scale = 1 / float(levelOfDetail);
	float u = 0;
	Point p12 = Point(p1->x / 3 + p2->x * 2 / 3, p1->y / 3 + p2->y * 2 / 3);
	Point p23 = Point(p2->x * 2 / 3 + p3->x / 3, p2->y * 2 / 3 + p3->y / 3);
	Point p34 = Point(p3->x, p3->y);
	Point p123 = Point(p12.x / 2 + p23.x / 2, p12.y / 2 + p23.y / 2);
	Point p234 = Point(p23.x, p23.y);
	Point p1234 = Point(p123.x, p123.y);
	Point last = p1234;
	int i;
	//draw segment
	for (i = 1; i <= levelOfDetail; i++) {
		u = scale * i;
		p12.x = p1->x * (1 - u) / 3 + p2->x * (2 + u) / 3;
		p12.y = p1->y * (1 - u) / 3 + p2->y * (2 + u) / 3;
		p23.x = p2->x * (2 - u) / 3 + p3->x * (1 + u) / 3;
		p23.y = p2->y * (2 - u) / 3 + p3->y * (1 + u) / 3;
		p34.x = p3->x * (1 - u / 3) + p4->x * u / 3;
		p34.y = p3->y * (1 - u / 3) + p4->y * u / 3;
		p123.x = p12.x * (1 - u) / 2 + p23.x * (1 + u) / 2;
		p123.y = p12.y * (1 - u) / 2 + p23.y * (1 + u) / 2;
		p234.x = p23.x * (1 - u / 2) + p34.x * (u / 2);
		p234.y = p23.y * (1 - u / 2) + p34.y * (u / 2);
		p1234.x = p123.x * (1 - u) + p234.x * u;
		p1234.y = p123.y * (1 - u) + p234.y * u;

		drawLine(last.x, last.y, p1234.x, p1234.y);
		last = p1234;
		if (i == levelOfDetail) {
			x = last.x;
			y = last.y;
		}
	}
	//then create a Point to be drawn where the knot should be

	Point p(x, y);
	p.draw();
}

#include "Bezier2.h"

//This function is provided to aid you.
//It should be used in the spirit of recursion, though you may choose not to.
//This function takes an empty vector of points, accum
//It also takes a set of control points, pts, and fills accum with
//the control points that correspond to the next level of detail.
void accumulateNextLevel(Pvector* accum, Pvector pts) {
	if (pts.empty()) return; 
	accum->push_back(*(pts.begin()));
	if (pts.size() == 1) return;
	for (Pvector::iterator it = pts.begin(); it != pts.end() - 1; it++) {
		/* YOUR CODE HERE  (only one to three lines)*/
		*it = Point((it->x) / 2 + ((it + 1)->x) / 2, (it->y) / 2 + ((it + 1)->y) / 2);
	}
	//save the last point
	Point last = *(pts.end()-1);
	pts.pop_back();
	//recursive call
	accumulateNextLevel(accum, pts);
	accum->push_back(last);
}


// The basic draw function for Bezier2.  Note that as opposed to Bezier, 
// this draws the curve by recursive subdivision.  So, levelofdetail 
// corresponds to how many times to recurse.  
void Bezier2::draw(int levelOfDetail) {
	//This is just a trick to find out if this is the top level call
	//All recursive calls will be given a negative integer, to be flipped here
	if (levelOfDetail > 0) {
		connectTheDots();
	} else {
		levelOfDetail = -levelOfDetail;
	}

	//Base case.  No more recursive calls.
	if (levelOfDetail <= 1) {
		if (points.size() >= 2) {
			for (Pvector::iterator it = points.begin(); it != points.end() - 1; it++) {

				/* YOUR CODE HERE */
				drawLine(it->x, it->y, (it + 1)->x, (it + 1)->y);
			}
		}
	} else {
		Pvector* accum = new Pvector();
		Bezier2 left, right;

		//add the correct points to 'left' and 'right'.
		//You may or may not use accum as you see fit.
		/* YOUR CODE HERE */
		accumulateNextLevel(accum, points);
		int i;
		Pvector::iterator iter = accum->begin();
		for (i = 0; i < accum->size(); i++) {
			if ( accum->size() % 2 == 0 && i < accum->size() / 2 || accum->size() % 2 != 0 && i <= (accum->size()-1)/2 ) {
				left.addPoint(accum->at(i).x, accum->at(i).y);
			}
			if(accum->size() % 2 == 0 && i > accum->size() / 2 || accum->size() % 2 != 0 && i >= (accum->size() - 1) / 2){
				right.addPoint(accum->at(i).x, accum->at(i).y);
			}
		}
		left.draw(1-levelOfDetail);
		right.draw(1-levelOfDetail);
		delete accum;
	}
}
