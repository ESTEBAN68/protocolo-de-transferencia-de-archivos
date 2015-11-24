struct proy_in
{
	/*input arg*/
	char* usuario;
	char* pass;
};
struct proy_in2
{
	/*input arg*/
	char* usuario;
	int token;
};
struct proy_out
{
	/*op result*/
	int token;
};
program PROY_PROG
{
	version PROY_VERS
	{
		proy_out PROYPROC(proy_in)=1; /*proc no=1*/
	}=1; /*version no*/
}=0x31540000;/*prog no*/