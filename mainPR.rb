# read argument
if ARGV.count != 1 
	puts "\n>> Please give a path of file...\n"
	exit
else
	filepath = ARGV[0] 
end

load "lib/PageRank.rb"

pr = PageRank.new(filepath, 0.85, 0.000001)
pr.path
pr.pagerank
pr.results

