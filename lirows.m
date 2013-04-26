function [Xsub,idx]=lirows(X,tol)
%
% Taken from: http://www.mathworks.com/matlabcentral/answers/49984
% Author: Matt J (link: http://www.mathworks.com/matlabcentral/answers/contributors/1440443-matt-j)
%
%Extract a linearly independent set of rows of a given matrix X
%
%    [Xsub,idx]=lirows(X)
%
%in:
%
%  X: The given input matrix
%  tol: A rank estimation tolerance. Default=1e-10
%
%out:
%
% Xsub: The extracted rows of X
% idx:  The indices (into X) of the extracted rows

permutation = randperm(size(X,1));
X = X(permutation,:);
X = X';
if ~nnz(X) %X has no non-zeros and hence no independent rows
   Xsub=[]; idx=[];
   return
end
if nargin<2, tol=1e-10; end
[Q, R, E] = qr(X,0); 
if ~isvector(R)
diagr = abs(diag(R));
else
diagr = R(1);   
end
%Rank estimation
r = find(diagr >= tol*diagr(1), 1, 'last'); %rank estimation
idx=sort(E(1:r));
Xsub=X(:,idx);

Xsub = Xsub';
idx = permutation(idx);