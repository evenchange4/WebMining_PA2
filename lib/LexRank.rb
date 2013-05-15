load "lib/VSM.rb"

class LexRank
	attr_accessor :path, :idf_path, :dictionary, :vectors, :idf, :graph_path

	def initialize(sentence_file_path, idf_file_path)
		@path = sentence_file_path
		@idf_path = idf_file_path
		@dictionary = Array.new
		@vectors = Array.new
	end

	def construct_VSMmodel
		puts "\n>> Constructing vector space mode..."
		make_dictionary
		make_vectors
		puts ">> done."
	end

	def graph(threshold)
		link = Array.new
		(0..@vectors.size-1).each do |i|
			tmp = []
			(0..@vectors.size-1).each do |j|
				if i != j && sim(i, j) > threshold
					tmp << j+1 # 先把 index 轉換成 對應的 sentence # 
				end
			end
			link << tmp
		end

		# output graph
		filepath = "#{File.dirname(@path)}/#{File.basename(@path)}.graph"
		@graph_path = filepath
		File.open(filepath,"w") do |f|
			f << "#maxnode #{link.size}\n"
			link.each_with_index { |value, index| f << "#{index+1}:#{value.size} #{value.join(" ")}\n" }
		end
		puts "\n>> Output \"graph\" file path >> \"#{filepath}\""
	end


	private
	# module 可直接當作 instance method 來 include
	include VSM

end
