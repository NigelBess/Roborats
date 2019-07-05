#ifndef LookUpTable_H
#define LookUpTable_H
class LookUpTable
{
  private:
  int** table;
  public:
  LookUpTable(int data[][2],int sizeOfData)
  {
    table = new int*[sizeOfData];
  }
  int getPoint(int value)
  {
    int n = sizeof(table)/sizeof(table[0]);
    for (int i = 0;i<n;i++)
    {
      if (table[i][0]>value)
      {
        if (i==0) return table[i][1]; 
        int m = (table[i][1]=table[i-1][1])(table[i][0]-table[i-1][0])*1000;
        int b = table[i][1]*1000-table[i][0]*m;
        return (m*value+b)/1000;
      }
    }
    return table[n][1];
  }
}
#endif
