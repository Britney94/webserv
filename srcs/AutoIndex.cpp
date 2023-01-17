#include "../includes/webserv.hpp"

static int	ft_count_digits(long n)
{
	int	count = 0;
	if (n < 0)
	{
		count++;
		n *= -1;
	}
	while (n > 0)
	{
		count++;
		n = n / 10;
	}
	return (count);
}

static char	*ft_itoa(int n)
{
	int		i;
	long	nb;
	char	*itoa;
	if (n == 0)
		return (strdup("0"));
	nb = n;
	i = ft_count_digits(nb);
	itoa = (char *)malloc(sizeof (char) * (i + 1));
	if (itoa == NULL)
		return (NULL);
	itoa[i--] = '\0';
	if (n < 0)
	{
		itoa[0] = '-';
		nb *= -1;
	}
	while (n > 0)
	{
		itoa[i] = '0' + (n % 10);
		n /= 10;
		i--;
	}
	return (itoa);
}

std::string	autoindex::renderPage(std::string directory, std::string path, int port, std::string host)
{
	DIR *dir;
	struct dirent *ent;
	int size;
	std::string page;
	char *tmp;
	page = "<html>\n";
	page.insert(page.size(), "<head><title>Index of ");
	page.insert(page.size(), host);
	page.insert(page.size(), "</title></head>\n");
	page.insert(page.size(), "<body bgcolor=\"white\">\n");
	page.insert(page.size(), "<h1>Index of ");
	page.insert(page.size(), host);
	page.insert(page.size(), "</h1><body><pre><hr>\n");
	if ((dir = opendir (path.c_str())) != NULL) {
		while ((ent = readdir (dir)) != NULL) {
			if (strcmp(".", ent->d_name) != 0 && strcmp("..", ent->d_name) != 0)
			{
				size = strlen(ent->d_name);
				page.insert(page.size(), "<a href=\"");
				page.insert(page.size(), "http://");
				page.insert(page.size(), host);
				tmp = ft_itoa(port);
				if (strcmp(tmp, "0") != 0){
					page.insert(page.size(), ":");
					page.insert(page.size(), tmp);
				}
				free(tmp);
				if (directory != ""){
					page.insert(page.size(), "/");
					page.insert(page.size(), directory);
				}
				page.insert(page.size(), "/");
				page.insert(page.size(), ent->d_name);
				page.insert(page.size(), "\">");
				page.insert(page.size(), ent->d_name);
				page.insert(page.size(), "</a>\n");
			}
		}
		closedir (dir);
	} 
	else { // CANNOT OPEN DIRECTORY
		perror ("Autoindex");
		page.clear();
		return "";
	}
	page.insert(page.size(), "</pre><hr></body>");
	page.insert(page.size(), "</html>\n");
	return page;
}
