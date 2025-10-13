

# include <ctype.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdint.h>
size_t	ft_strlen(const char *s)
{
	size_t	i;

	i = 0;
	while (s[i])
	{
		i++;
	}
	return (i);
}

int	ft_strcmp(const char *s1, const char *s2)
{
	int	i;

	i = 0;
	while (s1 && s2 && s1[i] && s2[i] && s1[i] == s2[i])
		i++;
	return (s1[i] - s2[i]);
} 
char	*ft_strdup(const char *s)
{
	int		i;
	char	*p;

	p = (char *)malloc((ft_strlen(s) + 1) * sizeof(char));
	if (!p)
		return (NULL);
	i = 0;
	while (s[i])
	{
		p[i] = s[i];
		i++;
	}
	p[i] = '\0';
	return (p);
}


void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*ptr;
	unsigned char	p;
	size_t			i;

	ptr = (unsigned char *)b;
	p = (unsigned char)c;
	i = 0;
	while (i < len)
	{
		ptr[i] = p;
		i++;
	}
	return (b);
}

void	*ft_calloc(size_t nmemb, size_t size)
{
	int		total_size;
	char	*p;

	if (size && SIZE_MAX / size < nmemb)
		return (NULL);
	total_size = nmemb * size;
	p = (char *)malloc(total_size);
	if (!p)
		return (NULL);
	if (nmemb == 0 || size == 0)
		total_size = 1;
	return (ft_memset(p, 0, total_size));
}

int	match_in_list(char *s1 , char **s2 ,char  **already_checked)
{
    int	i;
    int	j;
    int	checked;

	j = 0;
	checked = 0;
    if (!s1 || !s2)
        return 1;
    while (s2[j])
    {  
        i = 0;
        while (s1 && s2[j] && s1[i] && s2[j][i] && s1[i] == s2[j][i])
	    	i++;
        if(!(s1[i] - s2[j][i]))
        {
			if (!already_checked[checked])
			{
				already_checked[checked] = ft_strdup(s1);
				return 0;
			}
			while (already_checked[checked])
			{
				if (!ft_strcmp(s1 , already_checked[checked]))
					return 1;
				checked++;
			}
			already_checked[checked] = ft_strdup(s1);
			return 0;
        }
        j++;
    }
	return (1);
}




int main()
{
    char *s1 = "walid";
    char *s3 = "amine";
    char *s4 = "kamal";
    char  **already_checked = calloc(7 , sizeof(char *));
    char *s2[6] = {"wald" , "amine","amine", "kamal" ,"walid"};
    printf("%d \n%s\n" , match_in_list(s1 , s2 , already_checked) ,already_checked[0]);
    printf("%d \n%s\n" , match_in_list(s3 , s2 , already_checked) ,already_checked[1]);
    printf("%d \n%s\n" , match_in_list(s3 , s2 , already_checked) ,already_checked[2]);
}