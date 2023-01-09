/**************************************************************************************
	Fonction main du cgi -->  G�n�re une page web avec le nom et le ip
**************************************************************************************/

#include "Cgi.h"

void main()
{
	CCgi cxCgi;
	char tcBuf[256];

	//Ent�te html � ne pas oublier
	cout<<"content-type: text/html \n\n";

	cout<<"<html>\n<head><title>Page de test cgi</title></head>\n";
	cout<<"<body bgcolor =\"#d9d9d9\">\n";
	cout<<"<font color=\"#990000\" size = 20px>\n ";

	//Valider la m�thode d'envoi du formulaire
	if(cxCgi.GetMethod() == METHOD_ERR)
	{
		cout<<"Vous devez passer par le formulaire.\n";
		cout<<"</font>\n</body>\n</html>\n";
		return;
	}

	//Chargement des variables du formulaire
	if(!cxCgi.LoadFormVar())
	{
		cout<<"Une erreur est survenue.\n";
		cout<<"</font>\n</body>\n</html>\n";
		return;
	}

	//R�cup�ration de la valeur de la variable nom du formulaire
	cxCgi.GetVarValue("nom",tcBuf,256);
	cout<<"Bienvenu "<<tcBuf<<"\n";

	//R�cup�ration du ip
	cxCgi.GetClientIp(tcBuf,256);
	cout<<" dont le ip est  :  "<<tcBuf<<"\n";
	cout<<"</font>\n</body>\n</html>\n";
}