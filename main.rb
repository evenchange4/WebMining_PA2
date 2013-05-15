load "lib/PageRank.rb"
load "lib/LexRank.rb"

# PR = PageRank.new("../stanford-08-03.graph", damping_factor=0.85, epsilon=0.000001)
# PR = PageRank.new("../cs.stanford.2004-10.graph", 0.85, 0.000001)
# PR = PageRank.new("../4.graph", 0.85, 0.000001)
# PR = PageRank.new("../sample.sen.graph", damping_factor=0.85, epsilon=0.000001)
# PR.pagerank      # Transition Probability Matrix
# PR.sort_results
# PR.state       # final state at

lexRank = LexRank.new("../sample.sen", "../idf")
lexRank.construct_VSMmodel
lexRank.graph(threshold = 0.1)

pageRank2 = PageRank.new(lexRank.graph_path, 0.85, 0.000001)
pageRank2.pagerank
pageRank2.sort_results

# stanford = PageRank.new("../stanford-08-03.graph", 0.85, 0.000001)
# stanford.path
# stanford.pagerank
# stanford.results

# cs_stanford = PageRank.new("../cs.stanford.2004-10.graph", 0.85, 0.000001)
# cs_stanford.path
# cs_stanford.pagerank
# cs_stanford.results

# test = PageRank.new("../4.graph", 0.85, 0.000001)
# test.path
# test.pagerank
# test.results