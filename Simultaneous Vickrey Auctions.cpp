#include<vector>
#include<fstream>
#include<iostream>
#include<cstdlib>
#include<cstring>
#include<ctime>
#include<cmath>
#include<algorithm>
using namespace std;
#define eps 1e-13
#define eps1 1e-13

#define sgn(x) (fabs(x)<eps?0:(x>0?1:-1))
#define sgn1(x) (fabs(x)<eps1?0:(x>0?1:-1))

#define cmp(a,b) (a.y<b.y || (a.y==b.y && a.x<b.x))
#define equ(a,b) (sgn(a.x-b.x)==0 && sgn(a.y-b.y)==0)
#define MAXNUM 999999
ofstream fout ("10a.txt");
ofstream fout2 ("10b.txt");
ofstream fout3 ("10c.txt");
ofstream fout4 ("10d.txt");
ofstream fout5 ("10e.txt");
ofstream fout6 ("10f.txt");
ofstream fout7 ("10b1.txt");
ofstream fout8 ("10b2.txt");
ofstream fout9 ("10ab.txt");
ofstream fout10 ("10re.txt");

bool kao;
long double bid[1024][1024][2];
long double utility[1024][1024];
long double prob[1024][1024], xk[1024][1024], yk[1024][1024], zb[1024][1024];
bool s_l_[1024][1024], s__l[1024][1024], s_s_[1024][1024], s__s[1024][1024];
struct plane
{
    double origin_z;
    double x_plane_k;
    double y_plane_k;
};
struct point
{
    double x, y;
};
struct polygon
{
    vector<point> end_point;
    int siz;
    plane pl;
    double bid_1, bid_2;
};

double cross (point p0, point p1, point p2)
{
    return (p1.x - p0.x) * (p2.y - p0.y) - (p2.x - p0.x) * (p1.y - p0.y);
}

double dis(point p0, point p1)
{
    return sqrt((p0.x - p1.x) * (p0.x - p1.x) + (p0.y - p1.y) * (p0.y - p1.y));
}

long double det(point p0, point p1, point p2)
{
	return (p1.x - p0.x) * (p2.y - p0.y) - (p1.y - p0.y) * (p2.x - p0.x);
}

void Area_Polygon(int i, int j, const polygon &c)
{
    long double s = 0;
    for(int k = 1; k < c.end_point.size() - 1; k++)
    {
        s = s + 0.5 * det(c.end_point[0], c.end_point[k], c.end_point[k + 1]);
    }
    prob[i][j] = fabs(s);
    return ;
}

bool Point_in_Polygon(const polygon &c, point p)
{
    if(c.siz < 3)
        return false;

    for (int i = 0; i < c.end_point.size() - 1; i++)
    {
        if(sgn(c.end_point[i].x - p.x) == 0 && sgn(c.end_point[i].y - p.y) == 0)
            return true;
        if(sgn(c.end_point[i + 1].x - p.x) == 0 && sgn(c.end_point[i + 1].y - p.y) == 0)
            return true;
        if(sgn(c.end_point[i].x - c.end_point[i + 1].x) == 0)
        {
            if(sgn(c.end_point[i].x - p.x) == 0)
            {
                if(sgn(c.end_point[i].y - p.y) * sgn(c.end_point[i + 1].y - p.y) <= 0)
                    return true;
            }
            continue;
        }
        if(sgn(c.end_point[i].y - c.end_point[i + 1].y) == 0)
        {
            if(sgn(c.end_point[i].y - p.y) == 0)
            {
                if(sgn(c.end_point[i].x - p.x) * sgn(c.end_point[i + 1].x - p.x) <= 0)
                    return true;
            }
            continue;
        }
        if((sgn(c.end_point[i].y - p.y) * sgn(c.end_point[i + 1].y - p.y) <= 0) && (sgn(c.end_point[i].x - p.x) * sgn(c.end_point[i + 1].x - p.x) <= 0))
        {
            if(sgn((c.end_point[i].y - p.y) * (c.end_point[i + 1].x - p.x) - (c.end_point[i + 1].y - p.y) * (c.end_point[i].x - p.x) ) == 0)
                return true;
        }
    }

        if(sgn(c.end_point[0].x - c.end_point[c.end_point.size() - 1].x) == 0)
        {
            if(sgn(c.end_point[0].x - p.x) == 0)
            {
                if(sgn(c.end_point[0].y - p.y) * sgn(c.end_point[c.end_point.size() - 1].y - p.y) <= 0)
                    return true;
            }
        }
        else if(sgn(c.end_point[0].y - c.end_point[c.end_point.size() - 1].y) == 0)
        {
            if(sgn(c.end_point[0].y - p.y) == 0)
            {
                if(sgn(c.end_point[0].x - p.x) * sgn(c.end_point[c.end_point.size() - 1].x - p.x) <= 0)
                    return true;
            }
        }
        else if((sgn(c.end_point[0].y - p.y) * sgn(c.end_point[c.end_point.size() - 1].y - p.y) <= 0) && (sgn(c.end_point[0].x - p.x) * sgn(c.end_point[c.end_point.size() - 1].x - p.x) <= 0))
        {
            if(sgn((c.end_point[0].y - p.y) * (c.end_point[c.end_point.size() - 1].x - p.x) - (c.end_point[c.end_point.size() - 1].y - p.y) * (c.end_point[0].x - p.x)) == 0)
                return true;
        }
    bool f = false;
    int i, j;
    for(i = 0, j = c.siz -1; i < c.siz; j = i++)
    {
        if(sgn(c.end_point[i].y - p.y) * sgn(c.end_point[j].y - p.y) == -1)
        if((c.end_point[j].x - c.end_point[i].x) * (p.y - c.end_point[i].y) / (c.end_point[j].y - c.end_point[i].y) + c.end_point[i].x - p.x < 0)
        f = !f;
        if( sgn(c.end_point[i].y - p.y) == 0 && sgn(c.end_point[j].y - p.y) != 0)
        {
            if(c.end_point[i].x - p.x < 0)
                f = !f;
        }
        if(sgn(c.end_point[i].y - p.y) == 0 && sgn(c.end_point[j].y - p.y) == 0)
            return false;
    }

    if (!f)
        return f;
    f = false;
    for(i = 0, j = c.siz -1; i < c.siz; j = i++)
    {
        if(sgn(c.end_point[i].y - p.y) * sgn(c.end_point[j].y - p.y) == -1)
        if((c.end_point[j].x - c.end_point[i].x) * (p.y - c.end_point[i].y) / (c.end_point[j].y - c.end_point[i].y) + c.end_point[i].x - p.x > 0)
        f = !f;
        if(sgn(c.end_point[i].y - p.y) == 0 && sgn(c.end_point[j].y - p.y) != 0 )
        {
            if(c.end_point[i].x - p.x > 0)
                f = !f;
        }
        if(sgn(c.end_point[i].y - p.y) == 0 && sgn(c.end_point[j].y - p.y) == 0)
            return false;
    }

    return f;

}

bool sort_point(const point &v1, const point &v2)
{
    return cmp(v1, v2);
}

void Hull (polygon &c)
{
    sort(c.end_point.begin(), c.end_point.end(), sort_point);
    vector<point> tem_point, hull_point;
    int i = 0, j = 0, k;
    c.siz = c.end_point.size();
    vector<point>::iterator it, it1;
    for(;i < c.siz - 1; i++)
    {
        if(!equ(c.end_point[i], c.end_point[i + 1]))
        {
            tem_point.push_back(c.end_point [i]);
        }
    }
    if(!equ(c.end_point [i], c.end_point[0]))
        tem_point.push_back(c.end_point [i]);
    int n = tem_point.size();
    if (n < 3)
    {
        c.siz = tem_point.size();
        c.end_point = tem_point;
        return ;
    }
    it = tem_point.begin();
    it ++;
    it1 = it;
    it--;
    while(it1 != tem_point.end())
    {
        if(sgn((*it).x - (*it1).x) == 0 && sgn((*it).y - (*it1).y) == 0)
            it1 = tem_point.erase(it1);
        else
        {
            it++;
            it1++;
        }
    }

    n = tem_point.size();
    if (n < 3)
    {
        c.siz = tem_point.size();
        c.end_point = tem_point;
        return ;
    }
    for(i = 1; i < n - 1; i++)
    {
        for(j = i + 1; j < n; j++)
            if(cross(tem_point[0], tem_point[j], tem_point[i]) > 0 || (sgn(cross(tem_point[0], tem_point[j], tem_point[i]) == 0 && dis(tem_point[0], tem_point[j]) - dis(tem_point[0], tem_point[i]) < 0) ))
                swap(tem_point[i], tem_point[j]);

    }
    hull_point.push_back(tem_point[0]);
    hull_point.push_back(tem_point[1]);
    hull_point.push_back(tem_point[2]);


    for(i = 3; i < n; i++)
    {
        while (cross(hull_point[hull_point.size() - 2], tem_point [i], hull_point[hull_point.size() - 1]) >= 0)
        {
            it = hull_point.end();
            it --;
            hull_point.erase(it);
            if(hull_point.size() <= 1)
                break;
        }
        hull_point.push_back(tem_point[i]);
    }


    it = hull_point.begin();
    it++;
    while(it != hull_point.end())
    {
        it--;
        it1 = it;
        it++;
        if(sgn((*it).x - (*it1).x) == 0 && sgn((*it).y - (*it1).y) == 0)
            it = hull_point.erase(it);
        else
            it++;
    }
    c.siz = hull_point.size();
    c.end_point = hull_point;
}
plane pre_bid(double bid1, double bid2, vector <polygon> &pre_c, int x)
{
    plane tem;
    double s1 = 0, s2 = 0, s12 = 0, s1p = 0, s2p = 0, s12p = 0;
    int ii, jj;
    double i_, j_;
    i_ = bid1 * 128;
    j_ = bid2 * 128;
    ii = (int) i_;
    jj = (int) j_;
    if(x == 0)
    {
        s1 = bid1 * (1 - bid2);
        s2 = (1 - bid1) * bid2;
        s12 = bid1 * bid2;
        s1p = bid1 * s1 * 0.5;
        s2p = bid2 * s2 * 0.5;
        s12p = (bid1 + bid2) * s12 * 0.5;
    }
    else{
    for(int i = 0; i <= 64; i++)
    for(int j = 0; j <= 64; j++)
    {
        double bid1_op = (double)i / 64;
        double bid2_op = (double)j / 64;
        if(s_s_[ii][jj] && s__s[ii][jj])
        {

            if(bid1 > bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 == bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 > bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 > bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
        }
        else if(s_s_[ii][jj] && s__l[ii][jj])
        {
            if(bid1 > bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 == bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 > bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 == bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 > bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 == bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
        }
        else if(s_l_[ii][jj] && s__s[ii][jj])
        {
            if(bid1 > bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 == bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 == bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 > bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
        }
        else if(s_l_[ii][jj] && s__l[ii][jj])
        {
            if(bid1 > bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 == bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 == bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 > bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 == bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
        }
        else if(s_l_[ii][jj])
        {
            if(bid1 > bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 == bid2_op)
            {
                s12 += prob[i][j] * 0.5;
                s12p += (bid1_op + bid2_op) * 0.5 * prob[i][j];
                s1 += prob[i][j] * 0.5;
                s1p += bid1_op * 0.5 * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 == bid2_op)
            {
                s12 += prob[i][j] * 0.5;
                s12p += (bid1_op + bid2_op) * 0.5 * prob[i][j];
                s1 += prob[i][j] * 0.5;
                s1p += bid1_op * 0.5 * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 > bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 == bid2_op)
            {
                s2 += prob[i][j] * 0.5;
                s2p += bid2_op * 0.5 * prob[i][j];
            }
        }
        else if(s__l[ii][jj])
        {
            if(bid1 > bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 == bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j] * 0.5;
                s12p += (bid1_op + bid2_op) * 0.5 * prob[i][j];
                s2 += prob[i][j] * 0.5;
                s2p += bid2_op * 0.5 * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 == bid2_op)
            {
                s12 += prob[i][j] * 0.5;
                s12p += (bid1_op + bid2_op) * 0.5 * prob[i][j];
                s2 += prob[i][j] * 0.5;
                s2p += bid2_op * 0.5 * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j] * 0.5;
                s1p += bid1_op * 0.5 * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 > bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 == bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
        }
        else if(s_s_[ii][jj])
        {
            if(bid1 > bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 == bid2_op)
            {
                s12 += prob[i][j] * 0.5;
                s12p += (bid1_op + bid2_op) * 0.5 * prob[i][j];
                s1 += prob[i][j] * 0.5;
                s1p += bid1_op * 0.5 * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 > bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 == bid2_op)
            {
                s2 += prob[i][j] * 0.5;
                s2p += bid2_op * 0.5 * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 > bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 == bid2_op)
            {
                s2 += prob[i][j] * 0.5;
                s2p += bid2_op * 0.5 * prob[i][j];
            }
        }
        else if(s__s[ii][jj])
        {
            if(bid1 > bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 == bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j] * 0.5;
                s12p += (bid1_op + bid2_op) * 0.5 * prob[i][j];
                s2 += prob[i][j] * 0.5;
                s2p += bid2_op * 0.5 * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 == bid2_op)
            {
                s1 += prob[i][j] * 0.5;
                s1p += bid1_op * 0.5 * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j] * 0.5;
                s1p += bid1_op * 0.5 * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 > bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
        }
        else
        {
            if(bid1 > bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j];
                s12p += (bid1_op + bid2_op) * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 == bid2_op)
            {
                s12 += prob[i][j] * 0.5;
                s12p += (bid1_op + bid2_op) * 0.5 * prob[i][j];
                s1 += prob[i][j] * 0.5;
                s1p += bid1_op * 0.5 * prob[i][j];
            }
            if(bid1 > bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j];
                s1p += bid1_op * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 > bid2_op)
            {
                s12 += prob[i][j] * 0.5;
                s12p += (bid1_op + bid2_op) * 0.5 * prob[i][j];
                s2 += prob[i][j] * 0.5;
                s2p += bid2_op * 0.5 * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 == bid2_op)
            {
                s12 += prob[i][j] * 0.25;
                s12p += (bid1_op + bid2_op) * 0.25 * prob[i][j];
                s2 += prob[i][j] * 0.25;
                s2p += bid2_op * 0.25 * prob[i][j];
                s1 += prob[i][j] * 0.25;
                s1p += bid1_op * 0.25 * prob[i][j];
            }
            if(bid1 == bid1_op && bid2 < bid2_op)
            {
                s1 += prob[i][j] * 0.5;
                s1p += bid1_op * 0.5 * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 > bid2_op)
            {
                s2 += prob[i][j];
                s2p += bid2_op * prob[i][j];
            }
            if(bid1 < bid1_op && bid2 == bid2_op)
            {
                s2 += prob[i][j] * 0.5;
                s2p += bid2_op * 0.5 * prob[i][j];
            }
        }
    }
    }
    tem.origin_z = - (s1p + s2p + s12p);
    tem.x_plane_k = s1 + 0.75 * s12;
    tem.y_plane_k = s2 + 0.75 * s12;
    return tem;
}


bool parr(double kx, double ky, double bz, plane pl)
{
    if (sgn(kx - pl.x_plane_k) == 0 && sgn(ky - pl.y_plane_k) == 0)
    {
        return true;
    }
    return false;
}

int main()
{
    kao = false;
    vector<polygon> pre_c, c;
    memset(prob, 0, sizeof(prob));
    for(int i = 0; i < 128; i ++)
    for(int j = 0; j < 128; j ++)
    {
        polygon tem;
        point tem_point;
        tem_point.x = (double)i / 128;
        tem_point.y = (double)j / 128;
        tem.end_point.push_back(tem_point);
        tem_point.x = (double)(i + 1) / 128;
        tem_point.y = (double)j / 128;
        tem.end_point.push_back(tem_point);
        tem_point.x = (double)(i + 1) / 128;
        tem_point.y = (double)(j + 1) / 128;
        tem.end_point.push_back(tem_point);
        tem_point.x = (double)i / 128;
        tem_point.y = (double)(j + 1) / 128;
        tem.end_point.push_back(tem_point);
        tem.bid_1 = (double)i / 128;
        tem.bid_2 = (double)j / 128;
        tem.siz = 4;
        tem.pl.x_plane_k = 0;
        tem.pl.y_plane_k = 0;
        tem.pl.origin_z = 0;
        prob[i][j] = (double) 1 / 128 / 128;
        pre_c.push_back(tem);
    }
    memset(utility, 0, sizeof(utility));
    int x;
    for(x = 0; x < 3; x++)
    {
        polygon tem;
        point tem_point;
        tem_point.x = 0;
        tem_point.y = 0;
        tem.end_point.push_back(tem_point);
        tem_point.x = 0;
        tem_point.y = 1;
        tem.end_point.push_back(tem_point);
        tem_point.x = 1;
        tem_point.y = 1;
        tem.end_point.push_back(tem_point);
        tem_point.x = 1;
        tem_point.y = 0;
        tem.end_point.push_back(tem_point);
        tem.bid_1 = 0;
        tem.bid_2 = 0;
        tem.siz = 4;
        plane tem_plane;
        tem.pl.origin_z = 0;
        tem.pl.x_plane_k = 0;
        tem.pl.y_plane_k = 0;
        c.push_back(tem);

        double pre_xk = 0, pre_yk = 0, pre_zb = 0;
        memset(xk, 0, sizeof(xk));
        memset(yk, 0, sizeof(yk));
        memset(zb, 0, sizeof(zb));
        for(int i = 0; i <= 128; i++)
        for(int j = 0; j <= 128; j++)
        {
            //if(i == 3 && j == 2)
                //return 0;

            double bid_1, bid_2;
            bid_1 = (double)i / 128;
            bid_2 = (double)j / 128;
            tem_plane = pre_bid(bid_1, bid_2, pre_c, x);
            //cout << tem_plane.x_plane_k << ' ' << tem_plane.y_plane_k << ' ' << tem_plane.origin_z << ':' << endl;
            polygon tem_poly;
            tem_poly.bid_1 = bid_1;
            tem_poly.bid_2 = bid_2;
            tem_poly.pl = tem_plane;
            xk[i][j] = tem_plane.x_plane_k;
            yk[i][j] = tem_plane.y_plane_k;
            zb[i][j] = tem_plane.origin_z;
            if(sgn(tem_plane.x_plane_k) == 0 && sgn(tem_plane.y_plane_k) == 0)
                continue;
            if(sgn(tem_plane.x_plane_k - pre_xk) == 0 && sgn(tem_plane.y_plane_k - pre_yk) == 0 && sgn(tem_plane.origin_z - pre_zb) <= 0)
            {
                continue;
            }
            pre_xk = tem_plane.x_plane_k;
            pre_yk = tem_plane.y_plane_k;
            pre_zb = tem_plane.origin_z;
            vector<polygon>::iterator it = c.begin();
//            fout5 << i << ' ' << j << endl;
//           fout5 << tem_plane.x_plane_k << ' ' << tem_plane.y_plane_k << ' ' << tem_plane.origin_z << endl << endl;
//            cout << i << ' ' << j << ' ' << c.size() << endl;
            while(it != c.end())
            {
                for(int k = 0; k < (*it).end_point.size(); k++)
                {
                    if((*it).end_point[k].x < eps1)
                        (*it).end_point[k].x = 0;
                    if((*it).end_point[k].y < eps1)
                        (*it).end_point[k].y = 0;
                    if((*it).end_point[k].x > 1 - eps1)
                        (*it).end_point[k].x = 1;
                    if((*it).end_point[k].y > 1 - eps1)
                        (*it).end_point[k].y = 1;
                }
                (*it).siz = (*it).end_point.size();
                polygon current = (*it);
                double kx = current.pl.x_plane_k - tem_plane.x_plane_k;
                double ky = current.pl.y_plane_k - tem_plane.y_plane_k;
                double bz = current.pl.origin_z - tem_plane.origin_z;
 //               fout5 << current.pl.x_plane_k << ' ' << current.pl.y_plane_k << ' ' << current.pl.origin_z << endl;
                if(parr(tem_plane.x_plane_k, tem_plane.y_plane_k, tem_plane.origin_z, current.pl))
                {

 //                   fout5 << kx << ' ' << ky << ' ' << bz << endl;
 //                   cout << c.size() << endl;
                    if(bz < 0)
                    {
                        for(int l = 0; l < current.end_point.size(); l++)
                            tem_poly.end_point.push_back(current.end_point[l]);
                        it = c.erase(it);
                        continue;
                    }
                    else
                    {
                        it++;
                        continue;
                    }
                }
                polygon p1, p0;
//                cout << 1 << ' ';
                for(int k = 0; k < current.end_point.size() - 1; k++)
                {
 //                   fout5 << kx << ' ' << ky << ' ' << bz << endl;
 //                   fout5 << current.end_point[k].x << ' ' << current.end_point[k].y << ' ' << kx * current.end_point[k].x + ky * current.end_point[k].y + bz << endl;
 //                   fout5 << current.end_point[k].x * tem_plane.x_plane_k + current.end_point[k].y * tem_plane.y_plane_k + tem_plane.origin_z << endl;
 //                   fout5 << current.end_point[k].x * current.pl.x_plane_k + current.end_point[k].y * current.pl.y_plane_k + current.pl.origin_z << endl;
                    if(sgn(kx * current.end_point[k].x + ky * current.end_point[k].y + bz) == -1)
                        p0.end_point.push_back(current.end_point[k]);
                    if(sgn(kx * current.end_point[k].x + ky * current.end_point[k].y + bz) == 0)
                    {
                        p0.end_point.push_back(current.end_point[k]);
                        p1.end_point.push_back(current.end_point[k]);
                    }
                    if(sgn(kx * current.end_point[k].x + ky * current.end_point[k].y + bz) == 1)
                        p1.end_point.push_back(current.end_point[k]);
                    if(sgn(kx * current.end_point[k].x + ky * current.end_point[k].y + bz) * sgn(kx * current.end_point[k + 1].x + ky * current.end_point[k + 1].y + bz) == -1)
                    {//cout << 1 << ' ';
                        point inter;
                        if(sgn1(current.end_point[k].x - current.end_point[k + 1].x) == 0)
                        {
                            inter.x = (current.end_point[k].x + current.end_point[k + 1].x) / 2;
                            inter.y = (-bz - kx * inter.x) / ky;
                            //fout << i << ' ' << j << ' ' << inter.x <<' '<< inter.y << " a" << endl;

                        }
                        else if(sgn1(current.end_point[k].y - current.end_point[k + 1].y) == 0)
                        {
                            inter.y = (current.end_point[k].y + current.end_point[k + 1].y) / 2;
                            inter.x = (-bz - ky * inter.y) / kx;
                            //fout << i << ' ' << j << ' ' << inter.x <<' '<< inter.y << " b" << endl;
                        }
                        else
                        {
                            double temk, temb;
                            temk = (current.end_point[k + 1].y - current.end_point[k].y) / (current.end_point[k + 1].x - current.end_point[k].x);
                            temb = current.end_point[k + 1].y - current.end_point[k + 1].x * temk;
                            if(sgn1(ky) == 0)
                            {
                                inter.x = -bz / kx;
                                inter.y = temk * inter.x + temb;
                            }
                            else if (sgn1(kx == 0))
                            {
                                inter.y = -bz / ky;
                                inter.x = (inter.y - temb) / temk;
                            }
                            else
                            {
                                inter.x = (temb * ky - (-bz)) / (-(kx) - temk * ky);
                                inter.y = temk * inter.x + temb;
                            }
                            //fout << i << ' ' << j << ' ' << inter.x <<' '<< inter.y << " c " << temk << ' ' << ky << ' ' << current.end_point[k].x << ' ' << current.end_point[k+1].x << endl;

                        }
                        p0.end_point.push_back(inter);
                        p1.end_point.push_back(inter);
                    }
                }
 //                   fout5 << current.end_point[current.end_point.size() - 1].x << ' ' << current.end_point[current.end_point.size() - 1].y << ' ' << kx * current.end_point[current.end_point.size() - 1].x + ky * current.end_point[current.end_point.size() - 1].y + bz << endl;
 //                   fout5 << current.end_point[current.end_point.size() - 1].x * tem_plane.x_plane_k + current.end_point[current.end_point.size() - 1].y * tem_plane.y_plane_k + tem_plane.origin_z << endl;
 //                   fout5 << current.end_point[current.end_point.size() - 1].x * current.pl.x_plane_k + current.end_point[current.end_point.size() - 1].y * current.pl.y_plane_k + current.pl.origin_z << endl;
//                cout << 2 << ' ';
                if(sgn(kx * current.end_point[current.end_point.size() - 1].x + ky * current.end_point[current.end_point.size() - 1].y + bz) == -1)
                    p0.end_point.push_back(current.end_point[current.end_point.size() - 1]);
                if(sgn(kx * current.end_point[current.end_point.size() - 1].x + ky * current.end_point[current.end_point.size() - 1].y + bz) == 0)
                {
                    p0.end_point.push_back(current.end_point[current.end_point.size() - 1]);
                    p1.end_point.push_back(current.end_point[current.end_point.size() - 1]);
                }
                if(sgn(kx * current.end_point[current.end_point.size() - 1].x + ky * current.end_point[current.end_point.size() - 1].y + bz) == 1)
                    p1.end_point.push_back(current.end_point[current.end_point.size() - 1]);


                if(sgn(kx * current.end_point[current.siz - 1].x + ky * current.end_point[current.siz - 1].y + bz) * sgn(kx * current.end_point[0].x + ky * current.end_point[0].y + bz) == -1)
                    {
                        point inter;
                        if(sgn1(current.end_point[current.siz - 1].x - current.end_point[0].x) == 0)
                        {
                            inter.x = (current.end_point[current.siz - 1].x + current.end_point[0].x) / 2;
                            inter.y = (-bz - kx * inter.x) / ky;
                            //fout << i << ' ' << j << ' ' << inter.x <<' '<< inter.y << " d" << endl;

                        }
                        else if(sgn1(current.end_point[current.siz - 1].y - current.end_point[0].y) == 0)
                        {
                            inter.y = (current.end_point[current.siz - 1].y + current.end_point[0].y) / 2;
                            inter.x = (-bz - ky * inter.y) / kx;
                            //fout << i << ' ' << j << ' ' << inter.x <<' '<< inter.y << " e" << endl;
                        }
                        else
                        {
                            double temk, temb;
                            temk = (current.end_point[0].y - current.end_point[current.siz - 1].y) / (current.end_point[0].x - current.end_point[current.siz - 1].x);
                            temb = current.end_point[0].y - current.end_point[0].x * temk;
                            if(sgn1(ky) == 0)
                            {
                                inter.x = -bz / kx;
                                inter.y = temk * inter.x + temb;
                            }
                            else if (sgn1(kx == 0))
                            {
                                inter.y = -bz / ky;
                                inter.x = (inter.y - temb) / temk;
                            }
                            else
                            {
                                inter.x = (temb * ky - (-bz)) / (-(kx) - temk * ky);
                                inter.y = temk * inter.x + temb;
                            }
                            //fout << i << ' ' << j << ' ' << inter.x <<' '<< inter.y << " f" << endl;

                        }
                        p0.end_point.push_back(inter);
                        p1.end_point.push_back(inter);
                    }

 //               cout << 3 << ' ';
                if(p0.end_point.size() <= 2)
                {
                    it++;
                    continue;
                }
                else if(p1.end_point.size() <= 2)
                {
//                    cout << c.size() << endl;
                    for(int l = 0; l < current.end_point.size(); l++)
                            tem_poly.end_point.push_back(current.end_point[l]);
                        it = c.erase(it);
                        continue;

                }
                else
                {

                        for(int k = 0; k < p0.end_point.size(); k++)
                            tem_poly.end_point.push_back(p0.end_point[k]);
                        (*it).end_point = p1.end_point;
                        (*it).siz = p1.end_point.size();
                        it++;
                        continue;
                }
            }
//            cout << 7 << endl;
            if(!tem_poly.end_point.empty())
            {
                tem_poly.siz = tem_poly.end_point.size();
                Hull(tem_poly);
                //cout << tem_poly.end_point.size() << endl;
                if(tem_poly.end_point.size() >= 3)
                c.push_back(tem_poly);
                //fout << "yes\n";
                //for(int k = 0; k < tem_poly.end_point.size(); k++)
                //    fout << tem_poly.end_point[k].x << ',' << tem_poly.end_point[k].y << ';';
                //fout << endl;

            }
        }
        char ca;
        ca = 'A' + x;
        fout << ca << endl;
        memset(prob, 0, sizeof(prob));
/*        for(int k = 0; k < c.size(); k++)
        {
            fout << k << endl;
            fout << "points:" << endl;
            for(int i = 0; i < c[k].end_point.size(); i++)
                fout << '(' << c[k].end_point[i].x << ',' << c[k].end_point[i].y << ")\n";
            fout << "plane:" << endl;
            fout << c[k].pl.x_plane_k << ' ' << c[k].pl.y_plane_k << ' ' << c[k].pl.origin_z << endl;
            fout << "bids" << endl;
            fout << c[k].bid_1 << ' ' << c[k].bid_2 << endl;
        }*/
        fout4 << ca << endl;
        fout7 << ca << endl;
        fout8 << ca << endl;
        fout9 << ca << endl;
        fout10 << ca << endl;
        long double ss = 0;
        memset(s_s_, 0, sizeof(s_s_));
        memset(s__s, 0, sizeof(s__s));
        memset(s_l_, 0, sizeof(s_l_));
        memset(s__l, 0, sizeof(s__l));
        for(int k = 0; k < c.size(); k++)
        {
/*            int l = 0, m = c[k].end_point.size() - 1;
            fout4 << k << endl;
            fout7 << k << endl;
            fout8 << k << endl;
            for(; l < c[k].end_point.size(); m = l++)
            {
                fout4 << "line ([" << c[k].end_point[l].x << ", " << c[k].end_point[m].x << "], [" << c[k].end_point[l].y << ", " << c[k].end_point[m].y << "], [" << c[k].end_point[l].x * c[k].pl.x_plane_k + c[k].end_point[l].y * c[k].pl.y_plane_k + c[k].pl.origin_z << ", " << c[k].end_point[m].x * c[k].pl.x_plane_k + c[k].end_point[m].y * c[k].pl.y_plane_k + c[k].pl.origin_z << "])" << endl;
                fout7 << "line ([" << c[k].end_point[l].x << ", " << c[k].end_point[m].x << "], [" << c[k].end_point[l].y << ", " << c[k].end_point[m].y << "], [" << c[k].bid_1 << ", " << c[k].bid_1 << "])" << endl;
                fout8 << "line ([" << c[k].end_point[l].x << ", " << c[k].end_point[m].x << "], [" << c[k].end_point[l].y << ", " << c[k].end_point[m].y << "], [" << c[k].bid_2 << ", " << c[k].bid_2 << "])" << endl;
            }
            fout4 << endl;
            fout7 << endl;
            fout8 << endl;*/
            double i, j;
            i = c[k].bid_1 * 128;
            j = c[k].bid_2 * 128;
            Area_Polygon((int)i, (int)j, c[k]);
            ss = ss + prob[(int)i][(int)j];
        }
        for(int i = 0; i <= 128; i++)
        {
            for(int j = 0; j <= 128; j++)
                if(prob[i][j] > 0)
                {
                    s__s[i][j] = true;
                    break;
                }
            for(int j = 0; j <= 128; j++)
                if(prob[j][i] > 0)
                {
                    s_s_[j][i] = true;
                    break;
                }
            for(int j = 128; j >= 0; j--)
                if(prob[i][j] > 0)
                {
                    s__l[i][j] = true;
                    break;
                }
            for(int j = 128; j >= 0; j--)
                if(prob[j][i] > 0)
                {
                    s_l_[j][i] = true;
                    break;
                }
        }
        fout6 << ss << endl;
        double ab_ep = 0, re_ep = 0;
 /*       for(int i = 0; i < 64; i++)
        {
        cout << i << endl;
        for(int j = 0; j < 64; j++)
        {
            //cout << i << ' ' << j << endl;
            double val1, val2;
            val1 = (double)i / 64;
            val2 = (double)j / 64;
            point tem_point;
            tem_point.x = val1;
            tem_point.y = val2;
            int k;
            double pre_utility;
            double ki, kj;
            for(k = 0; k < pre_c.size(); k++)
            if(Point_in_Polygon(pre_c[k], tem_point))
            {
                ki = pre_c[k].bid_1 * 64;
                kj = pre_c[k].bid_2 * 64;
                break;
            }
            pre_utility = val1 * xk[(int)ki][(int)kj] + val2 * yk[(int)ki][(int)kj] + zb[(int)ki][(int)kj];
            for(k = 0; k < c.size(); k++)
            if(Point_in_Polygon(c[k], tem_point))
            {
                break;
            }
            if(k == c.size())
            {
                cout << "wrong!" << endl;
                return 0;
            }
            double tem_utility = val1 * c[k].pl.x_plane_k + val2 * c[k].pl.y_plane_k + c[k].pl.origin_z;
            if(tem_utility - pre_utility > ab_ep)
                ab_ep = tem_utility - pre_utility;
            if(pre_utility != 0)
            if((tem_utility - pre_utility) / pre_utility > re_ep)
                re_ep = (tem_utility - pre_utility) / pre_utility;
            fout9 << tem_utility - pre_utility << '\t';
            fout10 << (tem_utility - pre_utility) / pre_utility << '\t';
        }
        fout9 << endl;
        fout10 << endl;}*/
        fout2 << ab_ep << ' ' << re_ep << endl;
        pre_c = c;
        vector<polygon> ().swap(c);
    }
    return 0;
}
