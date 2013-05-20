# read argument
if ARGV.count != 1 
	puts "\n>> Please give a path of file... ex: $ \"ruby mainLR.rb sample.se\""
	exit
else
	filepath = ARGV[0] 
end

load "lib/PageRank.rb"
load "lib/LexRank.rb"

lr = LexRank.new(filepath, "idf")
lr.construct_VSMmodel
lr.graph(threshold = 0.3)

pr = PageRank.new(lr.graph_path, 0.85, 0.000001)
pr.pagerank
pr.sort_results
