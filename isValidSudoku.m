function valid = isValidSudoku(N, sudoku)

	N2 = N * N;

	valid = true;
	for i=1:N2
		row = sudoku(i,:);
		if( sum(sort(row(:)') ~= 1:N2) > 0 )
			disp(['Fails in row ' num2str(i)]);
			row
			valid = false;
			break;
		end

		col = sudoku(:,i);
		if( sum(sort(col(:)') ~= 1:N2) > 0 )
			disp(['Fails in col ' num2str(i)]);
			col
			valid = false;
			break;
		end

		blockRows = (1:N) + floor((i - 1) / N) * N;
		blockCols = (1:N) + mod((i - 1) , N) * N;
		block = sudoku( blockRows, blockCols );
		if( sum(sort(block(:)') ~= 1:N2) > 0 )
			disp(['Fails in block ' num2str(i)]);
			block
			valid = false;
			break;
		end
	end
end