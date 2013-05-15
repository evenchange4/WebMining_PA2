class PageRank
	attr_accessor :pagerank, :state, :path
	attr_reader :damp, :epsilon
	
	def initialize(graph_file_path, damping_factor, epsilon)
		@damp    = damping_factor
		@epsilon = epsilon
		@state   = 0
		@path = graph_file_path
		# 第一行，看有多少 node，故意多一格避免混亂
		File.open(@path) {|f| @pagerank = Array.new(f.readline.split(" ")[1].to_i+1, 1) } 
	end

	def pagerank
		# compute dangling page
		@dangling_page = Array.new(@pagerank.size, true)
		File.open(@path, "r").each do |line|
			# parse each line of file
			tmp1 = line.split(" ")
			tmp2 = tmp1[0].split(":")
			node_id    = tmp2[0].to_i
			out_degree = tmp2[1].to_i
			# 刪除有 outlink 的 page node
			@dangling_page[node_id] = false
		end
		# p @dangling_page

		puts ">> PageRanking..."
		begin
			@state += 1
			puts ">> run state #{@state}"
			@pre_pagerank = Array.new(@pagerank) # for Euclidean distance useage
			@sum = Array.new(@pagerank.size, 0)  # for pagerank useage
			
			File.open(@path, "r").each do |line|
				# parse each line of file
				tmp1 = line.split(" ")
				tmp2 = tmp1[0].split(":")
				node_id    = tmp2[0].to_i
				out_degree = tmp2[1].to_i

				# sum of pagerank value
				(1..out_degree).each do |i|
					node = tmp1[i].to_i
					@sum[node] += @pre_pagerank[node_id]/out_degree.to_f
				end 
			end

			# dangling page value
			@sum_of_dangling = 0
			@dangling_page.each_with_index do |value, index|
				if value && index != 0
					@sum_of_dangling += @pre_pagerank[index] / (@pagerank.size-1).to_f
				end
			end

			# dumping factor
			@pagerank = @sum.map{|e| (1-@damp) + @damp*@sum_of_dangling + @damp*e }

			# Euclidean distance for iterater
			dif = Math.sqrt(@pagerank.zip(@pre_pagerank).map { |x| (x[1] - x[0])**2 }.reduce(:+))
			# puts 
		end until dif < @epsilon
	end

	# output file，因為到時候給 LexRank 繼承的時候 輸出的格式不一樣
	def results
		filename = "#{File.dirname(@path)}/#{File.basename(@path)}.pagerank"
		File.open(filename,"w") do |f|
			@pagerank.delete_at(0) # skip index 0
			@pagerank.each_with_index {|value, index| f << "#{index+1}:#{value}\n" }
		end
		puts "\n>> output pagerank file path >> \"#{filename}\""
	end

	# output file，因為到時候給 LexRank 繼承的時候 輸出的格式不一樣
	def sort_results
		filename = "#{File.dirname(@path)}/#{File.basename(@path)}.pagerank"
		File.open(filename,"w") do |f|
			@pagerank.delete_at(0) # skip index 0
			f << "#{@pagerank.size}\n"
			pagerank_hash = Hash.new
			@pagerank.each_with_index {|value, index| pagerank_hash[index+1] = value }
			pagerank_hash.sort_by {|k,v| v }.reverse.map {|x| f << "#{x[0]}:#{x[1]}\n" }
		end
		puts "\n>> output \"pagerank\" file path >> \"#{filename}\""
	end
end


