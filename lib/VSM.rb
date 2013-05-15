module VSM
	# make dictionary into an Array
	def make_dictionary
		File.open(@path).each {|line| @dictionary << line.split(" ") }
		@dictionary.flatten!.sort!.uniq!
	end

	# TF-IDF vector
	def make_vectors
		# idf structure
		@idf = Array.new(@dictionary.size, Math.log(40252.0))
		File.open(@idf_path).each do |line|
			tmp = line.split(" ")
			term = tmp[0]
			idf_value = tmp[1].to_f
			if @dictionary.include?(term)
				@idf[@dictionary.index(term)] = idf_value
			end
		end

		File.open(@path).each_with_index do |line, index|
			# each line
			v = Array.new(@dictionary.size, 0)
			line.split(" ").each do |term|
				# each term
				v[@dictionary.index(term)] += 1
			end
			@vectors << v.zip(@idf).map!{|x|(x[1] * x[0])}
		end
		# @vectors		
	end

	# caculate cosine similarity
	def sim(vector1, vector2)
		# puts "\n>> Caculating cosine similarity of vectors[#{vector1}] and vectors[#{vector2}]"
		length1 = Math.sqrt(@vectors[vector1].reduce { |sum, n| sum + n**2 })
		length2 = Math.sqrt(@vectors[vector2].reduce { |sum, n| sum + n**2 })
		@vectors[vector1].zip(@vectors[vector2]).map { |x| (x[1] * x[0]) }.reduce { |sum, n| sum + (n / length1 / length2)}
	end
end