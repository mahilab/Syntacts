# include <cstdlib>
# include <iostream>
# include <ctime>

# include "pink_noise.hpp"

using namespace std;

//****************************************************************************80

void cdelay2 ( int m, int *q )

//****************************************************************************80
//
//  Purpose:
//
//    CDELAY2 is a circular buffer implementation of M-fold delay.
//
//  Example:
//
//    Suppose we call CDELAY2 12 times, always with M = 3, and with
//    Q having the input value 3 on the first call.  Q will go through
//    the following sequence of values over the 12 calls:
//
//    I   M  Qin  Qout
//
//    1   3   3   2
//    2   3   2   1
//    3   3   1   0
//    4   3   0   3
//    5   3   3   2
//    6   3   2   1
//    7   3   1   0
//    8   3   0   3
//    9   3   3   2
//   10   3   2   1
//   11   3   1   0
//   12   3   0   3
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 May 2010
//
//  Author:
//
//    Original C version by Sophocles Orfanidis.
//    This C++ version by John Burkardt.
//
//  Reference:
//
//    Sophocles Orfanidis,
//    Introduction to Signal Processing,
//    Prentice-Hall, 1995,
//    ISBN: 0-13-209172-0,
//    LC: TK5102.5.O246.
//
//  Parameters:
//
//    Input, int M, the maximum value that Q can have.
//
//    Input/output, int *Q, a counter which is decremented on every call.
//    However, the value "after" 0 is M.  
//
{
//
//  Decrement the offset.
//
  *q = *q - 1;
//
//  Q = - 1 wraps to Q = M.
//
  wrap2 ( m, q );

  return;
}
//****************************************************************************80

double *corr ( int n, double x[], int m )

//****************************************************************************80
//
//  Purpose:
//
//    CORR computes the sample correlation of a signal sample.
//
//  Discussion:
//
//    The sample correlation is defined, for 0 <= i < N, as
//
//      R(i) = 1/N * sum ( 0 <= j <= N - 1 - i ) X(i+j) * X(j)
//
//    The sample correlation is an estimate of the correlation function.
//
//    It is usually the case that the signal X is assumed to
//    have zero mean.  Here, we compute the mean and adjust the
//    calculation accordingly:
//
//      R(i) = 1/N * sum ( 0 <= j <= N - 1 - i ) 
//        ( X(i+j) - Xbar ) * ( X(j) - Xbar )
//
//    Experience suggests that only the first 5 or 10 percent of
//    the lags are statistically reliable, so that one might choose
//    M = N / 20 or M = N / 10, for instance.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 June 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Sophocles Orfanidis,
//    Introduction to Signal Processing,
//    Prentice-Hall, 1995,
//    ISBN: 0-13-209172-0,
//    LC: TK5102.5.O246.
//
//  Parameters:
//
//    Input, int N, the number of equally spaced signal
//    samples.
//
//    Input, double X[N], the signal samples.
//
//    Input, int M, the maximum lag to consider.
//    0 <= M < N.
//
//    Output, double CORR[M+1], the sample correlations.
//
{
  int i;
  int j;
  double *r;
  double xbar;

  r = new double[m+1];

  for ( i = 0; i <= m; i++ )
  {
    r[i] = 0.0;
  }

  xbar = 0.0;
  for ( j = 0; j < n; j++ )
  {
    xbar = xbar + x[j];
  }
  xbar = xbar / ( double ) ( n );

  for ( i = 0; i <= m; i++ )
  {
    for ( j = 0; j < n - i; j++ )
    {
      r[i] = r[i] + ( x[i+j] - xbar ) * ( x[j] - xbar );
    }
  }

  for ( i = 0; i <= m; i++ )
  {
    r[i] = r[i] / ( double ) ( n );
  }
  return r;
}
//****************************************************************************80

double *cross_corr ( int n, double x[], double y[], int m )

//****************************************************************************80
//
//  Purpose:
//
//    CROSS_CORR computes the sample cross correlation between two signal samples.
//
//  Discussion:
//
//    The sample cross correlation is defined, for 0 <= i < N, as
//
//      R(i) = 1/N * sum ( 0 <= j <= N - 1 - i ) X(i+j) * Y(j)
//
//    The sample cross correlation is an estimate of the cross 
//    correlation function.
//
//    It is usually the case that the signals X and Y are assumed to
//    have zero mean.  Here, we compute the means and adjust the
//    calculation accordingly:
//
//      R(i) = 1/N * sum ( 0 <= j <= N - 1 - i ) 
//        ( X(i+j) - Xbar ) * ( Y(j) - Ybar )
//
//    Experience suggests that only the first 5 or 10 percent of
//    the lags are statistically reliable, so that one might choose
//    M = N / 20 or M = N / 10, for instance.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    20 June 2010
//
//  Author:
//
//    John Burkardt
//
//  Reference:
//
//    Sophocles Orfanidis,
//    Introduction to Signal Processing,
//    Prentice-Hall, 1995,
//    ISBN: 0-13-209172-0,
//    LC: TK5102.5.O246.
//
//  Parameters:
//
//    Input, int N, the number of equally spaced signal
//    samples.
//
//    Input, double X[N], Y[N], the signal samples.
//
//    Input, int M, the maximum lag to consider.
//    0 <= M < N.
//
//    Output, double CROSS_CORR[M+1], the sample correlations.
//
{
  int i;
  int j;
  double *r;
  double xbar;
  double ybar;

  r = new double[m+1];

  for ( i = 0; i <= m; i++ )
  {
    r[i] = 0.0;
  }

  xbar = 0.0;
  for ( j = 0; j < n; j++ )
  {
    xbar = xbar + x[j];
  }
  xbar = xbar / ( double ) ( n );

  ybar = 0.0;
  for ( j = 0; j < n; j++ )
  {
    ybar = ybar + x[j];
  }
  ybar = ybar / ( double ) ( n );

  for ( i = 0; i <= m; i++ )
  {
    for ( j = 0; j < n - i; j++ )
    {
      r[i] = r[i] + ( x[i+j] - xbar ) * ( y[j] - ybar );
    }
  }

  for ( i = 0; i <= m; i++ )
  {
    r[i] = r[i] / ( double ) ( n );
  }

  return r;
}
//****************************************************************************80

double ran1f ( int b, double u[], int q[] )

//****************************************************************************80
//
//  Purpose:
//
//    RAN1F is a 1/F random number generator.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 May 2010
//
//  Author:
//
//    Original C version by Sophocles Orfanidis.
//    This C++ version by John Burkardt.
//
//  Reference:
//
//    Sophocles Orfanidis,
//    Introduction to Signal Processing,
//    Prentice-Hall, 1995,
//    ISBN: 0-13-209172-0,
//    LC: TK5102.5.O246.
//
//  Parameters:
//
//    Input, int B, the number of signals to combine.
//    For this algorithm, B cannot be more than 31!
//
//    Input/output, double U[B], the signals to combine.  It is expected
//    that each of the initial values of U will be drawn from a distribution
//    with zero mean.
//
//    Input/output, int Q[B], a set of counters that determine when each
//    entry of U is to be updated.
//
//    Output, double RAN1F, the value.
//
{
  int i;
  int j;
  double y;

  if ( 31 < b )
  {
    cerr << "\n";
    cerr << "RAN1F - Fatal error!\n";
    cerr << "  32 <= B, too many signals.\n";
    exit ( 1 );
  }

  y = 0.0;

  j = 1;
  for ( i = 0; i < b; i++ )
  {
    y = y + ranh ( j, u+i, q+i );
    j = j * 2;
  }
  if ( 0 < b )
  {
    y = y / ( double ) b;
  }

  return y;
}
//****************************************************************************80

double ranh ( int d, double *u, int *q )

//****************************************************************************80
//
//  Purpose:
//
//    RANH is a hold random number generator of period D.
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 May 2010
//
//  Author:
//
//    Original C version by Sophocles Orfanidis.
//    This C++ version by John Burkardt.
//
//  Reference:
//
//    Sophocles Orfanidis,
//    Introduction to Signal Processing,
//    Prentice-Hall, 1995,
//    ISBN: 0-13-209172-0,
//    LC: TK5102.5.O246.
//
//  Parameters:
//
//    Input, int D, the hold period.  D must be at least 1.
//
//    Input/output, double *U, a value to be held until Q has decremented
//    to 0, when Q will be reset to D, and U will be randomly reset.
//
//    Input/output, int *Q, a counter which is decremented by 1 on each call
//    until reaching 0.
//
//    Output, double RANH, the input value of U.
//
{
  double y;

  if ( d < 1 )
  {
    cerr << "\n";
    cerr << "RANH - Fatal error!\n";
    cerr << "  D < 1.\n";
    exit ( 1 );
  }
//
//  Hold this sample for D calls.
//
  y = *u;
//
//  Decrement Q and wrap mod D.
//
  cdelay2 ( d - 1, q );
//
//  Every D calls, get a new U with zero mean.
//
  if ( *q == 0 )
  {
    *u = 2.0 * ( double ) rand ( ) / ( double ) ( RAND_MAX ) - 1.0;
  }
  return y;
}
//****************************************************************************80

void timestamp ( )

//****************************************************************************80
//
//  Purpose:
//
//    TIMESTAMP prints the current YMDHMS date as a time stamp.
//
//  Example:
//
//    31 May 2001 09:45:54 AM
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    08 July 2009
//
//  Author:
//
//    John Burkardt
//
//  Parameters:
//
//    None
//
{
# define TIME_SIZE 40

  static char time_buffer[TIME_SIZE];
  const struct std::tm *tm_ptr;
  size_t len;
  std::time_t now;

  now = std::time ( NULL );
  tm_ptr = std::localtime ( &now );

  len = std::strftime ( time_buffer, TIME_SIZE, "%d %B %Y %I:%M:%S %p", tm_ptr );

  std::cout << time_buffer << "\n";

  return;
# undef TIME_SIZE
}
//****************************************************************************80

void wrap2 ( int m, int *q )

//****************************************************************************80
//
//  Purpose:
//
//    WRAP2 is a circular wrap of the pointer offset Q.
//
//  Discussion:
//
//    Input values of Q between 0 and M are "legal".
//    Values of Q below 0 are incremented by M + 1 until they are legal.
//    Values of Q above M are decremented by M + 1 until they become legal.
//    The legal value is the output value of the function.
//
//  Example:
//
//    M  Qin  Qout
//
//    3  -5   3
//    3  -4   0
//    3  -3   1
//    3  -2   2
//    3  -1   3
//    3   0   0
//    3   1   1
//    3   2   2
//    3   3   3
//    3   4   0
//    3   5   1
//    3   6   2
//    3   7   3
//    3   8   0
//
//  Licensing:
//
//    This code is distributed under the GNU LGPL license. 
//
//  Modified:
//
//    31 May 2010
//
//  Author:
//
//    Original C version by Sophocles Orfanidis.
//    This C++ version by John Burkardt.
//
//  Reference:
//
//    Sophocles Orfanidis,
//    Introduction to Signal Processing,
//    Prentice-Hall, 1995,
//    ISBN: 0-13-209172-0,
//    LC: TK5102.5.O246.
//
//  Parameters:
//
//    Input, int M, the maximum acceptable value for outputs.
//    M must be at least 0.
//
//    Input/output, int *Q, the value to be wrapped.
//
{
  if ( m < 0 )
  {
    cerr << "\n";
    cerr << "WRAP2 - Fatal error!\n";
    cerr << "  M < 0.\n";
    exit ( 1 );
  }
//
//  When Q = M + 1, it wraps to Q = 0.
//
  while ( m < *q )
  { 
    *q = *q - m - 1;
  }
//
//  When Q = - 1, it wraps to Q = M.
//
  while ( *q < 0 )
  {
    *q = *q + m + 1;
  }
  return;
}