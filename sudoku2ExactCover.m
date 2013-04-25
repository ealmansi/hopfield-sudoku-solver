function mat = sudoku2ExactCover(N)

	N2 = N * N;
	N4 = N2 * N2;
	
	rowOffset = 0;
	colOffset = rowOffset + N4;
	blockOffset = colOffset + N4;
	slotOffset = blockOffset + N4;
	
	mat = zeros(N4 * N2, 4 * N4);
	
	for row=1:N2
		for col=1:N2
			for num=1:N2
		
				block = floor((row - 1) / N) * N + floor((col - 1) / N) + 1;
		
				r = rowOffset + (row - 1) * N2 + num;
				c = colOffset + (col - 1) * N2 + num;
				b = blockOffset + (block - 1) * N2 + num;
				s = slotOffset + (row - 1) * N2 + col;
		
				indx = (row - 1) * N4 + (col - 1) * N2 + num;
				mat(indx, r) = 1;
				mat(indx, c) = 1;
				mat(indx, b) = 1;
				mat(indx, s) = 1;
			end
		end
	end
end