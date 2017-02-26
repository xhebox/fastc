#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <unistd.h>
#include <uv.h>
#include "vlq.h"

#define ERR(fmt, ...) do { fprintf(stderr, "\33[31mERR:\33[39m " fmt, ##__VA_ARGS__); exit(1); } while(0)
#define INFO(fmt, ...) fprintf(stdout, "\33[32mINFO:\33[39m " fmt, ##__VA_ARGS__);

static uv_loop_t* loop;

static uv_fs_t open_input;
static uv_fs_t read_input;
static uv_buf_t iov_input;

static uv_fs_t open_output;
static uv_fs_t write_output;

static uv_fs_t open_vql;
static uv_fs_t write_vql;


// reading buffer
static char buffer[1024];
// for storing output
static uint8_t n = 0;
static uint64_t w = 0;
static uint64_t len = 0;

static const char *progname = __FILE__;
const char *input = 0;
const char *output = 0;
const char *vlqput = 0;

static void usage(void) {
	fprintf(stdout,
		"Usage:\n"
		"  %s -i <input> -o <output> -v <vlqput>\n"
		"\n"
		"Options:\n"
		"  -i take a regular file as input, - is treated as stdin, fallback to stdin if no file is specific\n"
		"  -o similar to -i, fallback to stdout if no file is specific\n"
		"  -v similar to -i, fallback to stderr if no file is specific\n",
		progname);
	exit(1);
}

void _write(char* s, unsigned int len, int fd) {
	uv_buf_t ofov = uv_buf_init(s, len);
	int result;
	if ((result = uv_fs_write(loop, &write_output, fd, &ofov, 1, -1, NULL)) < 0)
		ERR("writing: %s\n", uv_strerror(result));
}

void _write_vlq() {
	// write vlq information
	int m;
	char buf[10];
	if ((m = to_vlq(len, buf)))
		_write(buf, m, open_vql.result);

	len = 0;
}

void _write_output(char* s) {
	// there're letters havn't been printed
	if( n > 0 && n != 4 ) {
		w++;
		n = 0;
	}

	// nothing to write
	if (w == 0) return;

	_write(s, w, open_output.result);

	w = 0;
}

void on_read(uv_fs_t *req) {
	if (req->result < 0) {
		ERR("reading: %s\n", uv_strerror(req->result));
	} else if (req->result == 0) {
		_write_vlq();
		uv_fs_t close;
		uv_fs_close(loop, &close, open_input.result, NULL);
		uv_fs_close(loop, &close, open_output.result, NULL);
		uv_fs_close(loop, &close, open_vql.result, NULL);
	} else {
		iov_input.len = req->result;

		char outbuf[256];
		for(int m = 0; m<iov_input.len; m++) {
			switch(iov_input.base[m]) {
			case 'A': 
				outbuf[w] |= (0x0)<<(6-n*2);
				len++;
				break;
			case 'U':
			case 'T':
				outbuf[w] |= (0x1)<<(6-n*2);
				len++;
				break;
			case 'G':
				outbuf[w] |= (0x2)<<(6-n*2);
				len++;
				break;
			case 'C':
				outbuf[w] |= (0x3)<<(6-n*2);
				len++;
				break;
			case 'R':
			case 'Y':
			case 'K':
			case 'M':
			case 'S':
			case 'W':
			case 'B':
			case 'D':
			case 'H':
			case 'V':
			case 'N':
			case '-':
				_write_output(outbuf);
				_write_vlq();
				_write(&iov_input.base[m], 1, open_output.result);
				break;
			case '\n':
			case ' ':
				continue;
			default: 
				ERR("encoding: meet an unknown character - %c, %x\n", iov_input.base[m], iov_input.base[m]);
				continue;
			}
			if( (++n) == 4) {
				w++;
				n = 0;
			}
		}

		// write down
		_write_output(outbuf);

		// keep reading
		int result;
		if ((result = uv_fs_read(loop, &read_input, open_input.result, &iov_input, 1, -1, on_read)) < 0)
				ERR("reading: %s\n", uv_strerror(result));
	}
}

int main(int argc, char** argv) {
	// init variables
	loop = uv_default_loop();

	// set progname
	progname = strrchr(argv[0], '/');
	if (progname)
		progname++;
	else
		progname = argv[0];

	// parsing args
	for (int c = 0; (c = getopt(argc, argv, "hv:i:o:")) != -1;) {
		switch (c) {
		case 'i':
			input = optarg;
			break;
		case 'o':
			output = optarg;
			break;
		case 'v':
			vlqput = optarg;
			break;
		default:
			usage();
		}
	}

	// open input
	int result = 0;
	if (input != 0) result = uv_fs_open(loop, &open_input, input, O_RDONLY, 0644, NULL);
	else open_input.result = 0;

	if (result < 0)
		ERR("opening in: %s\n", uv_strerror(result));

	// open output
	if (output != 0) result = uv_fs_open(loop, &open_output, output, O_WRONLY|O_CREAT, 0644, NULL);
	else open_output.result = 1;

	if (result < 0)
		ERR("opening out: %s\n", uv_strerror(result));

	// open vlqput
	if (vlqput != 0) result = uv_fs_open(loop, &open_vql, vlqput, O_WRONLY|O_CREAT, 0644, NULL);
	else open_vql.result = 2;

	if (result < 0)
		ERR("opening vlq: %s\n", uv_strerror(result));

	// set the loop
	iov_input = uv_buf_init(buffer, sizeof(buffer));
	result = uv_fs_read(loop, &read_input, open_input.result, &iov_input, 1, -1, on_read);
	if (result < 0)
		ERR("reading stdin: %s\n", uv_strerror(result));

	// run!
	uv_run(loop, UV_RUN_DEFAULT);

	// clean up
	uv_fs_req_cleanup(&open_input);
	uv_fs_req_cleanup(&read_input);
	uv_fs_req_cleanup(&open_output);
	uv_fs_req_cleanup(&write_output);
	uv_fs_req_cleanup(&open_vql);
	uv_fs_req_cleanup(&write_vql);
	return 0;
}
