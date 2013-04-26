% script parameters
sudokuSize = 3;
verbose = true;
itCount = 1000000;
Tp = 10;								% system temperature
TpRate = ((0.001)/(Tp))^(1/itCount);	% rate of cooling
Rf = 4;									% introduces randomness
RfRate = (1/Rf)^(1/itCount);				% rate of derandomization

% compute matrix representing sudoku problem in exact cover format
A = sudoku2ExactCover(sudokuSize);

n = size(A,1);
m = size(A,2);

% define Top, Iop 		[related to the quality function]
Top = - 2 * A * A';
Iop = - 4 * ones(n,1);

% define Tb, Ib       [weighted between quality and feasibility]
Tb = Top;
Ib = Iop;

% ensure Tb is symmetric and has 0's on its diagonal
Tb = 1/2 * (Tb + Tb');
Tb = Tb - diag(diag(Tb));

% define E(v) in terms of Tb and Ib
E = @(v) (- 1/2 * v' * Tb * v + v' * Ib);

% init state vector "v" with random values 0-1
v = rand(n,1);

for it=1:itCount
	% i: random index between 1 and length(v)
	i = randi(n);

	% compute neuron's input
	ui = Tb(i,:) * v - Ib(i) + Rf * (2 * rand() - 1);

	% update it's output in terms of its input and the system's temp (Tp)
	v(i) = 1/(1 + exp(-ui / Tp));

	% decrease Tp and Rf geometrically
	Tp = Tp * TpRate;
	Rf = Rf * beta;

	% optional: display E(v)
	if( verbose && mod(it,itCount/10) == 0)
		disp(['E(v) = ' num2str(E(v))]);
	end
end

sudoku = exactCover2Sudoku(sudokuSize, v)
isValid = isValidSudoku(sudokuSize, sudoku)