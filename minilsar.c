/**
 *  minilsar.c
 * 
 *  Copyright (C) 2019 NeverWin All Rights Reserved.
 *
 *  This file is part of miniLSAR.
 *
 *  miniLSAR is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  miniLSAR is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with miniLSAR.  If not, see <https://www.gnu.org/licenses/>.
 * 
 **/
 
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <getopt.h>
#include <archive.h>
#include <archive_entry.h>

static const char help_str[] = "\
minilsar %s, mini lsar clone.\n\
Usage: minilsar [options] archives\n\
\n\
Abailable Options:\n\
-help (-h)                      Display this information.\n\
-version (-v)                   Print version and exit.\n";


int main(int argc, char ** argv) {
    opterr = 0;
    static struct option long_options[] = {
        { "help", no_argument, NULL, 'h' },
        { "version", no_argument, NULL, 'v' },
        { NULL, 0, NULL, 0 }
    };
    int op;
    while (~(op = getopt_long_only(argc, argv, "vhlLtp:e:E:ij", long_options, NULL))) {
        switch (op) {
        case 'v':
            puts(VERSION);
            return EXIT_SUCCESS;
        case 'h':
        case '?':
            printf(help_str, VERSION);
            return op == '?' ? EXIT_FAILURE : EXIT_SUCCESS;
        }
    }
    if (optind >= argc) {
        printf(help_str, VERSION);
        return EXIT_FAILURE;
    }
    while (optind < argc) {
        printf("%s: ", argv[optind]);
        struct archive *a = archive_read_new();;
        struct archive_entry *entry;
        archive_read_support_filter_all(a);
        archive_read_support_format_all(a);
        int r = archive_read_open_filename(a, argv[optind++], 10240);
        if (r != ARCHIVE_OK) {
            puts("cannot open archive");
            return EXIT_FAILURE;
        }
        bool first = true;
        while (archive_read_next_header(a, &entry) == ARCHIVE_OK) {
            if (first) {
                puts(archive_format_name(a));
                first = false;
            }
            puts(archive_entry_pathname(entry));
            archive_read_data_skip(a);
        }
        r = archive_read_free(a);
        if (r != ARCHIVE_OK) {
            return EXIT_FAILURE;
        }
    }
    return EXIT_SUCCESS;
}
