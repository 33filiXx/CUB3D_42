

# include <ctype.h>
# include <fcntl.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <unistd.h>
# include <stdint.h>


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
    if (!already_checked[0])
        str
    if (!s1 || !s2)
        return 1;
    while (s2[j])
    {  
        i = 0;
        while (s1 && s2[j] && s1[i] && s2[j][i] && s1[i] == s2[j][i])
	    	i++;
        if(!(s1[i] - s2[j][i]))
        {

            while (already_checked)
	    	    i++;
			return 0;
        }
        j++;
    }
	return (1);
}




int main()
{
    char *s1 = "walid";
    char  **already_checked;
    char  **already_checked = ft_calloc(7 , sizeof(char *));
    char *s2[5] = {"wald" ,"kkaka", "walisd" ,"walid"};
    printf("%d" , match_in_list(s1 , s2 , already_checked));
}