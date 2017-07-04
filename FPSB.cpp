#include<vector>
#include<fstream>
#include<cstdlib>
#include<ctime>
#include<cmath>
#include<algorithm>
using namespace std;
ofstream fout1("a.txt");
ofstream fout2("b.txt");
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

void pred_bid (vector <seg> &q, vector <seg> &qg, seg *tem)
{
    double s0 = 0.000000;
    for(int i = 0; i < 100000; i++)
    {
        double val2, bid2;

        val2 = (double)rand()/RAND_MAX;

        vector<seg>::iterator it = find_if(q.begin(), q.end(), seg_between(val2));

        if(it == q.end())
            it--;

        bid2 = (*it).bid;

        if(bid2 < (*tem).bid)
        {
            s0 = s0 - (*tem).bid;
        }
    }
    s0 = s0 / 100000;
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
    for(float j = 0; j < 1; j += 0.01)
    {
        tem.be = j;
        tem.en = j + 0.01;
        tem.bid = j;
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


    for(int x = 0; x < 1; x++)
    {
        tem.be = 0;
        tem.en = 1;
        tem.bid = 0;
        tem.k = 0;
        tem.b = 0;
        p.push_back(tem);
        //tem.en = 2;
        //pg.push_back(tem);
        for(int i=0; i<500; i++)
        {
            tem.bid = (double)rand()/RAND_MAX;
            //If value is 0
            pred_bid(p_pre, pg_pre, &tem);
            tem.be = 0;
            tem.en = 0;
            //fout1 << "line( [" << 0 << ' ' << 1 << "],[" << tem.b << ' ' << tem.k + tem.b << "])" << endl;
            fout1 << tem.b << ' ' << tem.k << endl << tem.bid * tem.bid << ' ' << tem.bid << endl << endl;
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
                    it = pg.erase(it);
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
        double S = 0, V = 0; //area
        for (it = p.begin(); it != p.end(); it++)
        {
            //fout2 << (*it).be << '\t' << (*it).en << '\t' << (((*it).be * (*it).k + (*it).b) + ((*it).en * (*it).k + (*it).b))/2 << endl;
            fout2 << (*it).be << '\t' << (*it).en << '\t' << (*it).bid << endl;
        }
        fout2 << S << endl;
        p_pre = p;
        vector<seg> ().swap(p);
    }
}
