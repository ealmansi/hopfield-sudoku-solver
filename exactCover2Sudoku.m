function sudoku = exactCover2Sudoku(N, v)

	N2 = N * N;
	N4 = N2 * N2;

	sudoku = zeros(N2, N2);
	
	for row=1:N2
		for col=1:N2
			for num=1:N2
		
				indx = (row - 1) * N4 + (col - 1) * N2 + num;
				if( v(indx) == 0 )
					continue
				end

				sudoku(row, col) = num;
			end
		end
	end
end