#include <sys/stat.h>
#include <fcntl.h>
#include <sys/types.h>
#include <unistd.h>

//int isatty( int fd );

int main()
{
		int fd;
		printf("%d\n", isatty(0));

		fd = open("/dev/ttyS1", O_RDWR);
		printf("%d\n", isatty(fd));
		close(fd);

		fd = open("/dev/ttyS1", O_RDONLY);
		if (fd < -1)
		{
				printf("open error\n");
				exit(0);
		}
		printf("%d\n", isatty(fd));
		close(fd);
		//exit(0);
}
