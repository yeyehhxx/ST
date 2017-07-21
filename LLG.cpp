#include<vector>
#include<fstream>
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<cstring>
#include<cmath>
#include<algorithm>
using namespace std;
ofstream fout1("a_LLG3.txt");
ofstream fout2("b_LLG3.txt");
ofstream fout3("c_LLG3.txt");
ofstream fout4("d_LLG3.txt");
long double k_[2048], b_[2048];
struct seg
{
    double be,en;//represent the section of the segment
    double bid;
    double k,b;//geometric representation
};

class seg_between
{
	public:
	seg_between(double k) :m_k(k) {}
	bool operator() (std::vector <struct seg>::value_type&value)
	{
		return (value.be <= m_k && value.en > m_k);
	}
	private:
	double m_k;
};

bool sort_seg(const seg &v1, const seg &v2)
{
    return v1.be<v2.be;
}
/*
void pred_bid (vector <seg> *q, vector <seg> *qg, seg *tem)
{
    double s0 = 0.000000, s1 = 0.000000;
    for(int i = 0; i < 1000; i++)
    {
        double val2, valg, bid2, bidg;

        val2 = rand()%(9999+1)/(double)(9999+1);
        valg = rand()%(9999+1)/(double)(4999+1);

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

void pred_bid_local (vector <seg> &q, seg *tem)
{
    double s0 = 0.000000, s1 = 0.000000;
    vector<seg>::iterator it = q.begin();
    for(int i = 0; i < 512; i++)
    {
        double val2, bid2, valg, bidg, vcg1, vcg2, pay1, pay2;

        val2 = (double)i / 512;
        while (val2 >= (*it).en && i != 512)
            it++;

        bid2 = (*it).bid;

        for(int j = 0; j < 512; j++)
        {
            valg = (double)j / 512 * 2;

            bidg = valg;

            if(bid2 + (*tem).bid >= bidg)
            {
                vcg1 = max(0.0, bidg - bid2);
                vcg2 = max(0.0, bidg - (*tem).bid);
                pay1 = vcg1 + (bidg - vcg1 - vcg2) / 2;
                pay2 = vcg2 + (bidg - vcg1 - vcg2) / 2;
                s0 += pay1;
                s1 ++;
            }
        }
    }
    s0 = (-s0) / 512 / 512;
    s1 = s1 / 512 / 512;
    (*tem).b = s0;
    (*tem).k = s1;
    return ;
}

int main()
{
    vector<seg> p, p_pre, pg, pg_pre, p_keep, p_ep;
    srand(time(NULL));
    //initialization
    seg tem;
    for(float j = 0; j < 512; j = j + 1)
    {
        tem.be = j / 512;
        tem.en = (j + 1) / 512;
        tem.bid = (j + 0.5) / 512;
        tem.k = 0;
        tem.b = 0;
        p_pre.push_back(tem);
    }
    for(int x = 0; x < 15; x++)
    {
        //cout << x;
        tem.be = 0;
        tem.en = 1;
        tem.bid = 0;
        tem.k = 0;
        tem.b = 0;
        p.push_back(tem);
        memset(k_, 0, sizeof(k_));
        memset(b_, 0, sizeof(k_));
        //local player
        for(int i=1; i<512; i++)
        {
            tem.bid = (double)i/512;
            //If value is 0
            pred_bid_local(p_pre, &tem);
            tem.be = 0;
            tem.en = 0;
            //cout << i << endl;
            //fout1 << tem.bid << endl << tem.k << ' ' << tem.b << endl;
            k_[i] = tem.k;
            b_[i] = tem.b;
            p_keep = p;
            vector<seg> ().swap(p);
            vector<seg>::iterator it = p_keep.begin();
            //find if this utility line has highest utility in any range
            while(it != p_keep.end())
            {
                seg current;
                if(tem.k == (*it).k)
                {
                    tem.be = 1;
                    tem.en = 1;
                    //fout1 << 'a' << endl;
                    break;
                }
                if((tem.k * (*it).be + tem.b <= (*it).k * (*it).be + (*it).b)&&(tem.k * (*it).en + tem.b <= (*it).k * (*it).en + (*it).b))
                {
                    current = (*it);
                    p.push_back(current);
                    it++;
                    continue;
                }
                if((tem.k * (*it).be + tem.b > (*it).k * (*it).be + (*it).b)&&(tem.k * (*it).en + tem.b > (*it).k * (*it).en + (*it).b))
                {
                    current = (*it);
                    current.k = tem.k;
                    current.b = tem.b;
                    current.bid = tem.bid;
                    p.push_back(current);
                    it++;
                    continue;
                }
                if(tem.k * (*it).be + tem.b <= (*it).k * (*it).be + (*it).b)
                {
                    current = (*it);
                    current.en = ((*it).b - tem.b) / (tem.k - (*it).k);
                    p.push_back(current);
                    current.be = current.en;
                    current.en = (*it).en;
                    current.k = tem.k;
                    current.b = tem.b;
                    current.bid = tem.bid;
                    p.push_back(current);
                    it++;
                    continue;
                }
                if(tem.k * (*it).be + tem.b > (*it).k * (*it).be + (*it).b)
                {
                    current.be = (*it).be;
                    current.en = ((*it).b - tem.b) / (tem.k - (*it).k);
                    current.k = tem.k;
                    current.b = tem.b;
                    current.bid = tem.bid;
                    p.push_back(current);
                    current = (*it);
                    current.be = ((*it).b - tem.b) / (tem.k - (*it).k);
                    p.push_back(current);
                    it++;
                    continue;
                }
            }
            //fout2 << tem.be << ' ' << tem.en << endl;
            if(tem.be == 1 && tem.en == 1)
            {
                    //fout1 << 'c' << endl;
                    p = p_keep;
                    continue;
            }
            else
            {
                it = p.begin();
                vector<seg>::iterator it1 = p.begin();
                it1++;
                while(it1 != p.end())
                {
                    if((*it).bid == (*it1).bid)
                    {
                        (*it).en = (*it1).en;
                        it1 = p.erase(it1);
                    }
                    else
                    {
                        it++;

                        it1++;
                    }
                }
            }
            double S = 0, V = 0; //area
            for (it = p.begin(); it != p.end(); it++)
            {
                S += (((*it).be * (*it).k + (*it).b) + ((*it).en * (*it).k + (*it).b)) * ((*it).en - (*it).be) / 2;
                V += ((*it).en - (*it).be) * (*it).bid;
                //fout2 << (*it).be << ' ' << (*it).en << ' ';
            }
            //fout2 << endl;
            //fout2 << S << '\t' << V << endl;
        }
        fout2 << x << ": \n";
        fout3 << x << ": \n";
        vector<seg>::iterator it = p.begin();
/*        for (it = p.begin(); it != p.end(); it++)
        {
            //fout2 << (*it).be << '\t' << (*it).en << '\t' << (((*it).be * (*it).k + (*it).b) + ((*it).en * (*it).k + (*it).b))/2 << endl;
            fout2 << (*it).be << '\t' << (*it).en << '\t' << (*it).bid << endl;
            fout3 << "line ([" << (*it).be << ", " << (*it).en << "], [" << (*it).bid << ", " << (*it).bid << "])" << endl;
        }
        fout2 << endl;
        fout3 << endl;
*/
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
        fout4 << ep_ab << '\t' << ep_re;
        fout4 << endl;


        p_pre = p;
        vector<seg> ().swap(p);
    }
	return 0;
}
