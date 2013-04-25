% script parameters
sudokuSize = 3;
verbose = false;
itCount = 1000000;
Tp = 10;								% system temperature
alfa = ((10^-3)/(Tp))^(1/itCount);		% rate of cooling
Rf = 4;
beta = (1/Rf)^(1/itCount);				% rate of randomization

% compute matrix representing sudoku problem in exact cover format
A = sudoku2ExactCover(sudokuSize);
n = size(A,1);
m = size(A,2);

% define T, Ib 		[related to the quality function]
T = -2 * (A * A' - 4 * eye(n));
Ib = - 4 * ones(n,1);

% init state vector "v" with random values 0-1
v = rand(n,1);


for it=1:itCount
	% i: random index between 1 and length(v)
	i = randi(n);

	% compute neuron's input
	ui = T(i,:) * v - Ib(i) + Rf * (2 * rand() - 1);

	% update it's output in terms of its input and the system's temp (Tp)
	v(i) = 1/(1 + exp(-ui / Tp));

	% decrease Tp geometrically
	Tp = Tp * alfa;
	Rf = Rf * beta;


	% optional: display E(v)
	if( verbose )
		Ev = - 1/2 * v' * T * v - v' * Ib
	end
end

sudoku = exactCover2Sudoku(sudokuSize, v)
isValid = isValidSudoku(sudokuSize, sudoku)