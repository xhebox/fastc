#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <uv.h>

#define ERR(fmt, ...) fprintf(stderr, "\33[31mERR:\33[39m " fmt, ##__VA_ARGS__);
#define INFO(fmt, ...) fprintf(stdout, "\33[32mINFO:\33[39m " fmt, ##__VA_ARGS__);

uv_loop_t* loop;
uv_fs_t open_req;
uv_fs_t read_req;

static char buffer[1024];

static int n=0;
static char w=0;
static uv_buf_t iov;
static const char *progname = __FILE__;
static const char *input = "-";

static void usage(void) {
	fprintf(stdout,
		"Usage:\n"
		"  %s -i <input>\n"
		"\n"
		"Options:\n"
		"  -i take a regular file as input, - is treated as stdin, fallback to stdin if no file is specific\n",
		progname);
	exit(1);
}

void on_read(uv_fs_t *req) {
	if (req->result < 0) {
		ERR("reading: %s\n", uv_strerror(req->result));
	} else if (req->result == 0) {
		uv_fs_t close_req;
		uv_fs_close(loop, &close_req, open_req.result, NULL);
		// there're letters havnt been printed
		if( n > 0 && n != 4 )
			fprintf(stdout, "%c", w);
	} else {
		iov.len = req->result;

		// blank space is skiped
		for(int m=0; iov.base[m] && m<iov.len; m++) {
			switch(iov.base[m]) {
				case 'A': 
					w |= (0x0)<<(6-n*2);
					break;
				case 'U':
				case 'T':
					w |= (0x1)<<(6-n*2);
					break;
				case 'G':
					w |= (0x2)<<(6-n*2);
					break;
				case 'C':
					w |= (0x3)<<(6-n*2);
					break;
				default: 
					ERR("encoding: meet an unknown character - %c\n", iov.base[m]);
					continue;
			}
			if( (++n) == 4) {
				fprintf(stdout, "%c", w);
				n=w=0;
			}
		}

		// keep reading
		uv_fs_read(loop, &read_req, open_req.result, &iov, 1, -1, on_read);
	}
}

int main(int argc, char** argv) {

	int result;

	// set progname
	progname = strrchr(argv[0], '/');
	if (progname)
		progname++;
	else
		progname = argv[0];

	// set the loop
	loop = uv_default_loop();

	// parsing args
	for (int c=0; (c = getopt(argc, argv, "hvi:")) != -1;) {
		switch (c) {
		case 'i':
			input = optarg;
			break;
		default:
			usage();
			return 1;
		}
	}

	switch (*input) {
	case ' ':
		ERR("no input passed to %s\n", progname);
		usage();
		break;
	case '-':
		iov = uv_buf_init(buffer, sizeof(buffer));
		result = uv_fs_read(loop, &read_req, 0, &iov, 1, -1, on_read);
		if (result)
		    ERR("reading stdin: %s\n", uv_strerror(result));
		break;
	default:
		result = uv_fs_open(loop, &open_req, input, O_RDONLY, 0, NULL);
		if (result) {
		    iov = uv_buf_init(buffer, sizeof(buffer));
		    result = uv_fs_read(loop, &read_req, result, &iov, 1, -1, on_read);
		    if (result)
		        ERR("reading stdin: %s\n", uv_strerror(result));
		} else {
			ERR("opening file: %s\n", uv_strerror(result));
		}
		break;
	}

	// run!
	uv_run(loop, UV_RUN_DEFAULT);

	// clean up
	uv_fs_req_cleanup(&open_req);
	uv_fs_req_cleanup(&read_req);
	return 0;
}
