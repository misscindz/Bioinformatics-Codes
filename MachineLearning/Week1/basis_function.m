%parameter of gaussian (mu,s)-------------------
x=0:0.01:1;
num_gau = 5;
start = 0;
end_gau = 1;
mu(1) = 0;
s = 0.2;
%parameter of y(x,w) %---------------------------
%X=0:0.01:1;
r=rand(1,101);
y=sin(2*pi*x)+(r-0.5);
w= (rand(1,6)-0.5);
m=length(y);
alpha = 0.05;
%-----------------gaussian parameters-------------
for i = 2:1:num_gau
    mu(i) = mu(i-1) + (end_gau-start)/(num_gau-1);
    
end
Y = normpdf(x,mu(1),s); 
Y_1 = normpdf(x,mu(2),s); 
Y_2 = normpdf(x,mu(3),s); 
Y_3 = normpdf(x,mu(4),s); 
Y_4 = normpdf(x,mu(5),s); 
%plot(x,Y);hold on
%plot(x,Y_1);hold on
%plot(x,Y_2);hold on
%plot(x,Y_3);hold on
%plot(x,Y_4);
%axis([0 2 0 2])
%plot(normpdf(x));
%----------------linear regression-----------------


for iter = 1:1:1000
   for j = 1:1:length(w)
   sum = 0;
    for i = 1:1:m
        sum = sum + ( y(i) - (w(1)*1 + w(2)*exp(-(x(i)-mu(1)^2)/2*s^2) + w(3)*(exp(-(x(i)-mu(2)^2)/2*s^2))^2 + w(4)*(exp(-(x(i)-mu(3)^2)/2*s^2))^3+w(5)*(exp(-(x(i)-mu(4)^2)/2*s^2))^4 + w(6)*(exp(-(x(i)-mu(5)^2)/2*s^2))^5)) * (x(i)^(j-1)) ;  
    end
  
    w(j) = w(j) + alpha*sum;
    end
  
  error = 0;
  for i = 1:m
   
   yx(i) = w(1) * 1 + w(2) * x(i) + w(3) * (x(i)^2) + w(4) * (x(i)^3) + w(5) * (x(i)^4) + w(6) * (x(i)^5);
   error = error + (y(i) - yx(i))^2;
   
  end
  
  error
  err1(iter) = error;
end

plot(x,yx,'color','r'); hold on
%plot(x,y,'color','b');
xlabel('x')
ylabel('h(X)')

%plot(err1)
%axis([0 1000 0 50])
%xlabel('iterations')
%ylabel('error')

