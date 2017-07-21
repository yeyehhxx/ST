//random to take line, not sample randomly
#include<vector>
#include<fstream>
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<cstring>
#include<cmath>
#include<algorithm>
using namespace std;
ofstream fout1("a_nr.txt");
ofstream fout2("b_nr.txt");
ofstream fout3("c_nr.txt");
long double k_[2048], b_[2048];
struct seg
{
    long double be,en;//represent the section of the segment
    long double bid;
    long double k,b;//geometric representation
};

class seg_between
{
	public:
	seg_between(long double k) :m_k(k) {}
	bool operator() (vector <struct seg>::value_type&value)
	{
		return (value.be <= m_k && value.en > m_k);
	}
	private:
	long double m_k;
};

bool sort_seg(const seg &v1, const seg &v2)
{
    return v1.be<v2.be;
}
/*
void pred_bid (vector <seg> *q, vector <seg> *qg, seg *tem)
{
    long double s0 = 0.000000, s1 = 0.000000;
    for(int i = 0; i < 1000; i++)
    {
        long double val2, valg, bid2, bidg;

        val2 = rand()%(9999+1)/(long double)(9999+1);
        valg = rand()%(9999+1)/(long double)(4999+1);

        bid2 = val2;

        bidg = valg;

        if(bid2 + (*tem).bid > bidg)
        {
            s0 = s0 - bidg / (bid2 + (*tem).bid);
            s1 = s1 + 1 - bidg / (bid2 + (*tem).bid);
        }
    }
    s0 = s0 / 1000;
    s1 = s1 / 1000;
    (*tem).b = s0;
    (*tem).k = s1 - s0;
    return ;
}
*/

void pred_bid (vector <seg> &q, vector <seg> &qg, seg *tem)
{
    long double s0 = 0.000000, ss = 0.00000;
    vector<seg>::iterator it = q.begin();
    for(int i = 0; i < 1024; i++)
    {
        long double val2, bid2;

        val2 = (long double)(i+0.5)/1024;

        while (val2 >= (*it).en && i != 1023)
            it++;

        bid2 = (*it).bid;

        if(bid2 < (*tem).bid)
        {
            s0 = s0 - (*tem).bid * val2 * 2;
        }
    }
    s0 = s0 / 1024;
    (*tem).b = s0;
    (*tem).k = (-s0)/(*tem).bid;
    return ;
}

int main()
{
    vector<seg> p, p_pre, pg, pg_pre, p_keep;
    srand(time(NULL));
    //initialization
    seg tem;
    for(int j = 0; j < 512; j++)
    {
        tem.be = (long double)j/512;
        tem.en = (long double)(j+1)/512;
        tem.bid = (long double)(j+0.5)/512;
        tem.k = 1;
        tem.b = 0;
        p_pre.push_back(tem);
    }
/*    for(float j = 0; j < 2; j += 0.0001)
    {
        tem.be = j;
        tem.en = j + 0.0001;
        tem.bid = j;
        tem.k = 1;
        tem.b = 0;
        pg_pre.push_back(tem);
    }
*/


    for(int x = 0; x < 10; x++)
    {
        tem.be = 0;
        tem.en = 1;
        tem.bid = 0;
        tem.k = 0;
        tem.b = 0;
        p.push_back(tem);
        //tem.en = 2;
        //pg.push_back(tem);
        memset(k_, 0, sizeof(k_));
        memset(b_, 0, sizeof(k_));
        for(int i=0; i<= 512; i++)
        {
            tem.bid = (long double)i/ (long double)512;
            //If value is 0
            if(tem.bid == 0) continue;
            pred_bid(p_pre, pg_pre, &tem);
            tem.be = 0;
            tem.en = 0;
            //fout1 << "line( [" << 0 << ' ' << 1 << "],[" << tem.b << ' ' << tem.k + tem.b << "])" << endl;
            fout1 << tem.b << ' ' << tem.k << endl << tem.bid * tem.bid << ' ' << tem.bid << endl << endl;
            k_[i] = tem.k;
            b_[i] = tem.b;
            vector<seg>::iterator it = p.begin();
            p_keep = p;
            while(it != p.end())
            {
                if(tem.k == (*it).k)
                {
                    tem.be = 0;
                    tem.en = 0;
                    break;
                }
                if((*it).be == (*it).en)
                    it = p.erase(it);
                if((tem.k * (*it).be + tem.b <= (*it).k * (*it).be + (*it).b)&&(tem.k * (*it).en + tem.b <= (*it).k * (*it).en + (*it).b))
                {
                    it++;
                    continue;
                }
                if((tem.k * (*it).be + tem.b > (*it).k * (*it).be + (*it).b)&&(tem.k * (*it).en + tem.b > (*it).k * (*it).en + (*it).b))
                {
                    //fout1 << 'b' << ' ';
                    tem.en = (*it).en;
                    it = p.erase(it);
                    continue;
                }
                if(tem.k * (*it).be + tem.b <= (*it).k * (*it).be + (*it).b)
                {
                    tem.be = ((*it).b - tem.b) / (tem.k - (*it).k);
                    tem.en = (*it).en;
                    (*it).en = tem.be;
                    //fout1 << 'a' << ' ';
                    it++;
                    continue;
                }
                if(tem.k * (*it).be + tem.b > (*it).k * (*it).be + (*it).b)
                {
                    tem.en = ((*it).b - tem.b) / (tem.k - (*it).k);
                    (*it).be = tem.en;
                    //fout1 << 'c' << endl;
                    break;
                }
            }
            //fout2 << tem.be << ' ' << tem.en << endl;
            if(tem.be ==  tem.en)
            {
                p = p_keep;
                continue;
            }
            else
                p.push_back(tem);
            std:sort(p.begin(), p.end(), sort_seg);

            /*for (it = p.begin(); it != p.end(); it++)
            {
                S += (((*it).be * (*it).k + (*it).b) + ((*it).en * (*it).k + (*it).b)) * ((*it).en - (*it).be) / 2;
                V += ((*it).en - (*it).be) * (*it).bid;
                //fout2 << (*it).be << ' ' << (*it).en << ' ';
            }*/
            //fout2 << endl;
            //fout2 << S << '\t' << V << endl;
        }
        fout2 << x << ": \n";
        vector<seg>::iterator it = p.begin();
        long double S = 0, V = 0; //area
        for (it = p.begin(); it != p.end(); it++)
        {
            //fout2 << (*it).be << '\t' << (*it).en << '\t' << (((*it).be * (*it).k + (*it).b) + ((*it).en * (*it).k + (*it).b))/2 << endl;
            fout2 << (*it).be << '\t' << (*it).en << '\t' << (*it).bid << endl;
        }
        //fout2 << S << endl;
        double s = 0.000, ep_ab = 0, ep_re = 0;
        it = p.begin();
        vector<seg>::iterator it2 = p_pre.begin();
        while(it != p.end() && it2 != p_pre.end())
        {
            double t_be, t_en;
            t_be = max ((*it).be, (*it2).be);
            t_en = min ((*it).en, (*it2).en);
            if((*it).en <= t_be)
            {
                it++;
                continue;
            }
            if((*it2).en <= t_be)
            {
                it2++;
                continue;
            }
            double temp = (*it2).bid * 512;
            //cout << (int) temp << ' ' << k_[(int) temp] << ' ' << b_[(int) temp] << endl;
            if( ((*it).k * t_be + (*it).b) - (k_[(int)temp] * t_be + b_[(int)temp]) > ep_ab)
            {
                ep_ab = ((*it).k * t_be + (*it).b) - (k_[(int)temp] * t_be + b_[(int)temp]);
            }
            if( ((*it).k * t_en + (*it).b) - (k_[(int)temp] * t_en + b_[(int)temp]) > ep_ab)
            {
                ep_ab = ((*it).k * t_en + (*it).b) - (k_[(int)temp] * t_en + b_[(int)temp]);
            }
            if ((k_[(int)temp] * t_be + b_[(int)temp]) != 0)
            if ((((*it).k * t_be + (*it).b) - (k_[(int)temp] * t_be + b_[(int)temp]))/(k_[(int)temp] * t_be + b_[(int)temp]) > ep_re)
            {
                ep_re = (((*it).k * t_be + (*it).b) - (k_[(int)temp] * t_be + b_[(int)temp]))/(k_[(int)temp] * t_be + b_[(int)temp]);
            }
            if ((k_[(int)temp] * t_en + b_[(int)temp]) != 0)
            if ((((*it).k * t_en + (*it).b) - (k_[(int)temp] * t_en + b_[(int)temp]))/(k_[(int)temp] * t_en + b_[(int)temp]) > ep_re)
            {
                ep_re = (((*it).k * t_en + (*it).b) - (k_[(int)temp] * t_en + b_[(int)temp]))/(k_[(int)temp] * t_en + b_[(int)temp]);
            }
            if((*it).be <= (*it2).be)
                it++;
            else
                it2++;
        }
        fout3 << ep_ab << '\t' << ep_re;
        fout3 << endl;
        p_pre = p;
        vector<seg> ().swap(p);
    }
}
