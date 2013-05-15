### read argument
#read argument
if ARGV.count != 1 
	puts "\n>> Please give a path of file...\n"
	exit
else
	filepath = ARGV[0] 
end

load "lib/PageRank.rb"

# PR = PageRank.new("stanford-08-03.graph", damping_factor=0.85, epsilon=0.000001)
# PR = PageRank.new("cs.stanford.2004-10.graph", 0.85, 0.000001)
# PR = PageRank.new("4.graph", 0.85, 0.000001)
# PR = PageRank.new("sample.sen.graph", damping_factor=0.85, epsilon=0.000001)
# PR.pagerank      # Transition Probability Matrix
# PR.sort_results
# PR.state       # final state at


# stanford = PageRank.new("stanford-08-03.graph", 0.85, 0.000001)
# stanford.path
# stanford.pagerank
# stanford.results

cs_stanford = PageRank.new("cs.stanford.2004-10.graph", 0.85, 0.000001)
cs_stanford.path
cs_stanford.pagerank
cs_stanford.results

