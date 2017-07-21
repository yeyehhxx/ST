#include<vector>
#include<fstream>
#include<iostream>
#include<cstdlib>
#include<ctime>
#include<cstring>
#include<cmath>
#include<algorithm>
using namespace std;
ofstream fout1("a_LLG2.txt");
ofstream fout2("b_LLG2.txt");
ofstream fout3("c_LLG2.txt");
ofstream fout4("d_LLG2.txt");
//correlation parameter is 0.5.
long double k_[2048], b_[2048];

struct seg
{
    double be,en;//represent the section of the segment
    double bid;
    double k,b;//geometric representation
    double k1,b1;//correlation part
    double k_,b_;
};

class seg_between
{
	public:
	seg_between(double k) :m_k(k) {}
	bool operator() (vector <struct seg>::value_type&value)
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



void pred_bid_local (vector <seg> &q, seg *tem)
{
    double s0 = 0.000000, s1 = 0.000000;
    vector<seg>::iterator it = q.begin();
    double val2, bid2, valg, bidg, vcg1, vcg2, pay1, pay2;

    for(int i = 0; i <= 1024; i++)
    {

        val2 = (double)i / 1024;
        while (val2 >= (*it).en && i != 1024)
            it++;

        bid2 = (*it).bid;

        for(int j = 0; j <= 1024; j++)
        {
            valg = (double)j / 1024 * 2;

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
    s0 = (-s0) / 1024 / 1024;
    s1 = s1 / 1024 / 1024;
    (*tem).b = s0 * 0.5;
    (*tem).k = s1 * 0.5;

    it = q.begin();
    s0 = 0;
    s1 = 0;
    while(it != q.end())
    {
        bid2 = (*it).bid;
        s0 = 0;
        s1 = 0;
        for(int j = 0; j <= 1024; j++)
        {
            valg = (double)j / 1024 * 2;

            bidg = valg;

            if((*tem).bid + (*it).bid >= bidg)
            {
                vcg1 = max(0.0, bidg - bid2);
                vcg2 = max(0.0, bidg - (*tem).bid);
                pay1 = vcg1 + (bidg - vcg1 - vcg2) / 2;
                pay2 = vcg2 + (bidg - vcg1 - vcg2) / 2;
                s0 += pay1;
                s1 ++;
            }
        }

        s0 = s0 / 1024;
        (*it).k1 = s1 / 1024 / 2;
        (*it).b1 = -s0 / 2;
        if((*it).bid == (*tem).bid)
        {
            (*it).k_ = (*it).k1 + (*tem).k;
            (*it).b_ = (*it).b1 + (*tem).b;
        }
        it++;
    }

    return ;
}


int main()
{
    vector<seg> p, p_pre, pg, pg_pre, p_keep;
    srand(time(NULL));
    //initialization
    seg tem;
    for(float j = 0; j < 1024; j = j + 1)
    {
        tem.be = j / 1024;
        tem.en = (j + 1) / 1024;
        tem.bid = (j + 0.5) / 1024;
        tem.k = 0;
        tem.b = 0;
        p_pre.push_back(tem);
    }

    for(int x = 0; x < 20; x++)
    {
        //cout << x;
        tem.be = 0;
        tem.en = 1;
        tem.bid = 0;
        tem.k = 0;
        tem.b = 0;
        memset(k_, 0, sizeof(k_));
        memset(b_, 0, sizeof(k_));
        p.push_back(tem);
        //local player
        for(int i = 0; i<1024; i++)
        {
            tem.bid = (double)i/1024;
            //If value is 0
            pred_bid_local(p_pre, &tem);
            tem.be = 0;
            tem.en = 0;
            //cout << i << ' ';
            k_[i] = tem.k;
            b_[i] = tem.b;
            //fout1 << tem.bid << endl << tem.k << ' ' << tem.b << endl;
            p_keep = p;
            vector<seg> ().swap(p);
            vector<seg>::iterator it = p_keep.begin();
            vector<seg>::iterator it_p = p_pre.begin();

            //find if this utility line has highest utility in any range
            while(it != p_keep.end())
            {
                if((*it).be == (*it).en)
                    it = p_keep.erase(it);
                seg current;
                current.be = max((*it).be, (*it_p).be);
                current.en = min((*it).en, (*it_p).en);
                current.k = tem.k + (*it_p).k1;
                current.b = tem.b + (*it_p).b1;
                //fout4 << tem.k << ' ' << (*it_p).k1 << ' ' << tem.b << ' ' << (*it_p).b1 << endl;
                current.bid = tem.bid;
                //cout << current.be << ' ' << current.en << endl;
                if(current.be >= current.en)
                {
                    if((*it).en <= (*it_p).en)
                        it++;
                    else
                        it_p++;
                    continue;
                }
                if(current.k == (*it).k && current.b == (*it).b)
                {
                    p.push_back(current);
                    if((*it).en <= (*it_p).en)
                        it++;
                    else
                        it_p++;
                    continue;
                }
                if((current.k * current.be + current.b <= (*it).k * current.be + (*it).b)&&(current.k * current.en + current.b <= (*it).k * current.en + (*it).b))
                {
                    current.k = (*it).k;
                    current.b = (*it).b;
                    current.bid = (*it).bid;
                    p.push_back(current);
                    if((*it).en <= (*it_p).en)
                        it++;
                    else
                        it_p++;
                    continue;
                }
                if((current.k * current.be + current.b > (*it).k * current.be + (*it).b)&&(current.k * current.en + current.b > (*it).k * current.en + (*it).b))
                {
                    p.push_back(current);
                    if((*it).en <= (*it_p).en)
                        it++;
                    else
                        it_p++;
                    continue;
                }
                if(current.k * current.be + current.b <= (*it).k * current.be + (*it).b)
                {
                    double tem_be = ((*it).b - current.b) / (current.k - (*it).k), tem_en = current.en, tem_k = current.k, tem_b = current.b, tem_bid = current.bid;
                    current.en = tem_be;
                    current.k = (*it).k;
                    current.b = (*it).b;
                    current.bid = (*it).bid;
                    p.push_back(current);
                    current.be = tem_be;
                    current.en = tem_en;
                    current.k = tem_k;
                    current.b = tem_b;
                    current.bid = tem_bid;
                    p.push_back(current);
                    if((*it).en <= (*it_p).en)
                        it++;
                    else
                        it_p++;
                    continue;
                }
                if(current.k * current.be + current.b > (*it).k * current.be + (*it).b)
                {
                    double tem_be = ((*it).b - tem.b) / (tem.k - (*it).k);
                    double tem_en = current.en;
                    current.en = tem_be;
                    p.push_back(current);
                    current.be = tem_be;
                    current.en = tem_en;
                    current.k = (*it).k;
                    current.b = (*it).b;
                    current.bid = (*it).bid;
                    p.push_back(current);
                    if((*it).en <= (*it_p).en)
                        it++;
                    else
                        it_p++;
                    continue;
                }
            }
            double S = 0, V = 0; //area
            it = p.begin();
            vector<seg>::iterator it1 = it+1;
            while(it1 != p.end())
            {
                if ((*it).be == (*it).en)
                {
                    it = p.erase(it);
                    it1 = it+1;
                    continue;
                }
                if((*it).k == (*it1).k && (*it).b == (*it1).b && (*it).bid == (*it1).bid)
                {
                    (*it1).be = (*it).be;
                    it = p.erase(it);
                    it1 = it+1;
                    continue;
                }
                it++;
                it1++;
            }
            //fout2 << endl;
            //fout2 << S << '\t' << V << endl;
        }
        vector<seg>::iterator it = p.begin();
        vector<seg>::iterator it1 = it + 1;
        while(it1 != p.end())
        {
            if ((*it).be == (*it).en)
            {
                it = p.erase(it);
                it1 = it+1;
                continue;
            }
            if((*it).bid == (*it1).bid)
            {
                (*it1).be = (*it).be;
                it = p.erase(it);
                it1 = it+1;
                continue;
            }
            it++;
            it1++;
        }
        //global player
        //fout2 << x << ": \n";
        fout3 << x << ": \n";
        it = p.begin();
        for (it = p.begin(); it != p.end(); it++)
        {
            //fout2 << (*it).be << '\t' << (*it).en << '\t' << (((*it).be * (*it).k + (*it).b) + ((*it).en * (*it).k + (*it).b))/2 << endl;
       //     fout2 << (*it).be << '\t' << (*it).en << '\t' << (*it).bid << '\t' << (*it).k * (*it).be + (*it).b << '\t' << (*it).k * (*it).en + (*it).b << endl;
            fout3 << "line ([" << (*it).be << ", " << (*it).en << "], [" << (*it).bid << ", " << (*it).bid << "])" << endl;
        }
        //fout2 << endl;
        fout3 << endl;
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
            double temp = (*it2).bid * 1024;
            //cout << (int) temp << ' ' << k_[(int) temp] << ' ' << b_[(int) temp] << endl;
            if( ((*it).k * t_be + (*it).b) - ((*it2).k_ * t_be + (*it2).b_) > ep_ab)
            {
                ep_ab = ((*it).k * t_be + (*it).b) - ((*it2).k_ * t_be + (*it2).b_);
            }
            if( ((*it).k * t_en + (*it).b) - ((*it2).k_ * t_en + (*it2).b_) > ep_ab)
            {
                ep_ab = ((*it).k * t_en + (*it).b) - ((*it2).k_ * t_en + (*it2).b_);
            }
            if (((*it2).k_ * t_be + (*it2).b_) != 0)
            if ((((*it).k * t_be + (*it).b) - ((*it2).k_ * t_be + (*it2).b_))/((*it2).k_ * t_be + (*it2).b_) > ep_re)
            {
                ep_re = (((*it).k * t_be + (*it).b) - ((*it2).k_ * t_be + (*it2).b_))/((*it2).k_ * t_be + (*it2).b_);
            }
            if (((*it2).k_ * t_en + (*it2).b_) != 0)
            if ((((*it).k * t_en + (*it).b) - ((*it2).k_ * t_en + (*it2).b_))/((*it2).k_ * t_en + (*it2).b_) > ep_re)
            {
                ep_re = (((*it).k * t_en + (*it).b) - ((*it2).k_ * t_en + (*it2).b_))/((*it2).k_ * t_en + (*it2).b_);
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
}
