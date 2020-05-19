#!/usr/bin/python3

import subprocess

output_filename = 'build_version_auto.h'

def git_version():
	return subprocess.check_output(['git', 'log', '--pretty=format:%H', '-n', '1']).decode('ascii')

if __name__ == '__main__':
	import argparse, re, collections, functools, os, textwrap
	from string import Template
	
	parser = argparse.ArgumentParser(description = "Generates git snapshot headers.",
			usage = "%(prog)s [options]")
	
	parser.add_argument('source_dir', help="Source directory")
	parser.add_argument('output_dir', help="Output directory")
	
	args = parser.parse_args()
	
	source_directory = os.path.abspath(os.path.normpath(args.source_dir))
	output_directory = os.path.normpath(args.output_dir)
	with open(os.path.join(output_directory, output_filename), 'wt') as f:
		values = {
			'git_hash': git_version(),
			'git_short_hash': git_version()[:8]
		}
		f.write(textwrap.dedent("""
			#if defined _auto_version_included
				#endinput
			#endif
			#define _auto_version_included
			
			#define GIT_COMMIT_HASH "{git_hash}"
			#define GIT_COMMIT_SHORT_HASH "{git_short_hash}"
		""".format(**values))[1:])
