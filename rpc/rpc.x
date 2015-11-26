struct proy_in
{
	char usuario[256];
	char pass[256];
};
struct proy_in2
{
	/*input arg*/
	char usuario[256];
	int token;
};
struct rta
{
	int rta;
};
program PROY_PROG
{
	version PROY_VERS
	{
		rta login(proy_in)=1; 
		rta logout(proy_in2)=2;

	}=1; 
}=0x31540000;