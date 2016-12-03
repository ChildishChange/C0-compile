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
	float m;
	int fib_array[20];
	i= 20;
	m = 0.1;
	m = m+++20;
	printf(m);
	m = m-+-20;
	printf(m);
	m = m---20;
	printf(m);
	m = m-++20;
	printf(m);
	m = m--+20;
	printf(m);
	m = m*-+20;
	printf(m);
	scanf(n);
	
	while(n>=i)
	{
		scanf(n);
			
	}
	n = fun(n);
	for(i=0;i<n;i=i+1)
	{
		fib_array[i] = fib(i+1);
	}
	printf("THE FIB ARRAY IS:\n");
	for(i = 0;i<n;i = i+1)
	{
		printf("NO.",i);
		printf(fib_array[i]);
		
	}
	return;
}
