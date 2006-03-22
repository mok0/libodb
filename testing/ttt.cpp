#include	<iostream.h>
main()
{
	int 	x=25,y=50;
	cout << "x = ";
	cout.width(5);
	cout.fill(' ');
	cout << x << ", y = ";
	cout.width(5);
	cout.fill('0');
	cout << y << "\n";
}
