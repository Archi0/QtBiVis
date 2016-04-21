#include "common.h"

void qs(QVector<double> *tab, int left, int right, QStringList* list )
{
    int i = left;
    int j = right;
    double x = (*tab)[( left + right ) / 2 ];
    do
    {
        while( (*tab)[ i ] < x )
             i++;

        while( (*tab)[ j ] > x )
             j--;

        if( i <= j )
        {
            std::swap((*tab)[i],(*tab)[j]);
            std::swap((*list)[i],(*list)[j]);
            i++;
            j--;
        }
    } while( i <= j );

    if( left < j ) qs( tab, left, j , list);

    if( right > i ) qs( tab, i, right, list );

}
void qs2(QVector<int> *tab, int left, int right, QVector<double> *list )
{
    int i = left;
    int j = right;
    int x = (*tab)[( left + right ) / 2 ];
    do
    {
        while( (*tab)[ i ] < x )
             i++;

        while( (*tab)[ j ] > x )
             j--;

        if( i <= j )
        {
            std::swap((*tab)[i],(*tab)[j]);
            std::swap((*list)[i],(*list)[j]);
            i++;
            j--;
        }
    } while( i <= j );

    if( left < j ) qs2( tab, left, j , list);

    if( right > i ) qs2( tab, i, right, list );

}

QVector<double> CalcSigma(QList<QStringList> *values, QList<QString> *biclusters, QVector<double> *outAvg)
{
   QVector<double> val(biclusters->size());
   for(int nB = 0; nB < biclusters->size(); nB++)
   {
       QStringList split = biclusters->at(nB).split("|");
       QString tempRows = split[0];
       QString tempCols = split[1];
       QStringList rows = tempRows.split(QRegExp("\\s+"));
       rows.removeAll("");
       QStringList cols = tempCols.split(QRegExp("\\s+"));
       cols.removeAll("");
       QVector<double> avg(cols.size()), varr(cols.size()), sig(cols.size());
       avg.fill(0);
       varr.fill(0);
       sig.fill(0);
       for(int i=0; i<cols.size(); i++)
       {
           for(int j=0; j<rows.size(); j++)
           {
               avg[i]+=(*values)[rows.at(j).toInt()].at(i).toDouble();
           }
           avg[i]=avg[i]/(double)rows.size();
           for(int j=0; j<rows.size(); j++)
           {
               varr[i]+=(((*values)[rows.at(j).toInt()].at(i).toDouble()-avg[i])*((*values)[rows.at(j).toInt()].at(i).toDouble()-avg[i]));
           }
           varr[i]=varr[i]/(double)rows.size();
           sig[i]=sqrt(varr[i]);
       }

       for(int i=0;i<sig.size();i++)
       {
           val[nB]+=sig[i];
           (*outAvg)[nB] +=avg[i];
       }
       val[nB] = val[nB]/(double)sig.size();
       (*outAvg)[nB] = (*outAvg)[nB]/(double)avg.size();
   }

   return val;
}

void qs3(QVector<double> *tab, int left, int right, QVector<int> *list)
{
    int i = left;
    int j = right;
    double x = (*tab)[( left + right ) / 2 ];
    do
    {
        while( (*tab)[ i ] < x )
             i++;

        while( (*tab)[ j ] > x )
             j--;

        if( i <= j )
        {
            std::swap((*tab)[i],(*tab)[j]);
            std::swap((*list)[i],(*list)[j]);
            i++;
            j--;
        }
    } while( i <= j );

    if( left < j ) qs3( tab, left, j , list);

    if( right > i ) qs3( tab, i, right, list );
}
