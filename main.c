#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <uv.h>

uv_loop_t* loop;
uv_fs_t open_req;
uv_fs_t read_req;

static char buffer[1024];

static int n=0;
static char w=0;
static uv_buf_t iov;
static const char *progname = __FILE__;
static const char *input = "no";

static void usage(void) {
	printf("Usage:\n"
		"  %s [-i <input>]\n",
		progname);
	exit(1);
}

void on_read(uv_fs_t *req) {
	if (req->result < 0) {
		fprintf(stderr, "Read error: %s\n", uv_strerror(req->result));
	}
	else if (req->result == 0) {
		uv_fs_t close_req;
		uv_fs_close(loop, &close_req, open_req.result, NULL);
		if( n > 0 && n != 4 )
			fprintf(stdout, "%c", w);
	}
	else if (req->result > 0) {
		iov.len = req->result;
		for(int m=0; iov.base[m] && m<iov.len; m++) {
			switch(iov.base[m]) {
				case 'A': 
					w |= (0x0)<<(6-n*2);
					break;
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
					continue;
			}
			if( (++n) == 4) {
				fprintf(stdout, "%c", w);
				n=w=0;
			}
		}
		uv_fs_read(loop, &read_req, open_req.result, &iov, 1, -1, on_read);
	}
}

void on_open(uv_fs_t *req) {
	assert(req == &open_req);
	if (req->result >= 0) {
		iov = uv_buf_init(buffer, sizeof(buffer));
		uv_fs_read(loop, &read_req, req->result,
				&iov, 1, -1, on_read);
	}
	else {
		fprintf(stderr, "error opening file: %s\n", uv_strerror((int)req->result));
	}
}

int main(int argc, char** argv) {

	// set progname
	progname = argv[0];

	// parsing args
	int c;
	while ((c = getopt(argc, argv, "hvi:")) != -1) {
		switch (c) {
		case 'i':
			input = optarg;
			break;
		default:
			usage();
			return 1;
		}
	}

	if(strcmp(input,"no") == 0) {
		fprintf(stderr, "err: there's no input file passed to %s\n", progname);
		usage();
	}

	// set the loop
	loop = uv_default_loop();
	uv_fs_open(loop, &open_req, input, O_RDONLY, 0, on_open);

	// run!
	uv_run(loop, UV_RUN_DEFAULT);

	uv_fs_req_cleanup(&open_req);
	uv_fs_req_cleanup(&read_req);
	return 0;
}
