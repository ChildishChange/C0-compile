int fun(int i)
{
	if(i==2)
		return (2);
	else
		return (i*fun(i-1));
}
int fib(int i)
{
	if(i<=0)
	{	
		printf("FUCK YOU ! YOU SON OF A BITCH!\n");
		return (-1);
	}
	else
	{	
		if(i<=2)
			return (1);
		else
			return (fib(i-1)+fib(i-2));
	}
}
void main()
{
	int i,n;
	int fib_array[20];
	scanf(n);
	while(n>=20)
	{
		scanf(n);
			
	}
	for(i=0;i<n;i=i+1)
	{
		fib_array[i] = fib(i+1);
	}
	printf("THE FIB ARRAY IS:\n");
	for(i = 0;i<n;i = i+1)
	{
		printf("NO.",i);
		printf("\t");
		printf(fib_array[i]);
		printf("\n");
	}
	return;
}
