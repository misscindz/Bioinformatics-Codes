clc 
close all
clear all
feature = zeros(60,2);
feature = importdata('data_k_means.mat');
max_f = max(feature);
max_x = max_f(1);
max_y = max_f(2);
min_f = min(feature);
min_x = min_f(1);
min_y = min_f(2);
diff_y = max_y - min_y;
diff_x = max_x - min_x;
centroid_1 = [diff_x/3,diff_y/3];
centroid_2 = [2*(diff_x/3), 2*(diff_y/3)];
centroid_3 = [3*(diff_x/3), 3*(diff_y/3)];
 x = feature(:,1);
 y = feature(:,2);
x_1 = centroid_1(1);
y_1 = centroid_1(2);
x_2 = centroid_2(1);
y_2 = centroid_2(2);
x_3 = centroid_3(1);
y_3 = centroid_3(2);
 scatter(x,y);hold on
distance_11 = 0;
distance_21 = 0;
distance_31 = 0;
%scatter(x_1,y_1); hold on scatter(x_2,y_2); hold on scatter(x_3,y_3); hold
%on

cluster1 = zeros(20,2);
cluster2 = zeros(20,2);
cluster3 = zeros(20,2);
%dist_cent1 = zeros(60);
j = 1;
p = 1;
q = 1;
center1 = zeros(1,2);
center2 = zeros(1,2);
center3 = zeros(1,2);
%while (center1(1,1) ~= x_1) & (center1(1,2) ~=y_1) & (center2(1,1) ~= x_2)
%& (center2(1,2)~= y_2) & (center3(1,1) ~= x_3) & (center3(1,2)~=y_3) t = 1
for iter = 1:1:100
    
     j = 1;
     p = 1;
     q = 1;
    for i = 1:1:length(feature)
        dist_cen1(i) = sqrt((x_1-x(i))^2+(y_1-y(i))^2);
        dist_cen2(i) = sqrt((x_2-x(i))^2+(y_2-y(i))^2);
        dist_cen3(i) = sqrt((x_3-x(i))^2+(y_3-y(i))^2);
        if dist_cen1(i) < dist_cen2 & dist_cen1(i) < dist_cen3(i) 
         
            cluster1(j,1) = x(i);
            cluster1(j,2) = y(i);
            j= j+1;
        elseif dist_cen2(i) < dist_cen1(i) & dist_cen2(i)< dist_cen3(i)
             cluster2(q,1) = x(i);
             cluster2(q,2) = y(i);
             q= q+1;
        elseif dist_cen3(i) < dist_cen1(i) & dist_cen3(i) < dist_cen2(i)
             cluster3(p,1) = x(i);
             cluster3(p,2) = y(i);
            p= p+1;
         
        end
    end

center1 = mean(cluster1);
center2 = mean(cluster2);
center3 = mean(cluster3);
if (center1(1,1) == x_1) & (center1(1,2) ==y_1) & (center2(1,1) == x_2) & (center2(1,2)~= y_2) & (center3(1,1) == x_3) & (center3(1,2)==y_3)
    break;
else
    x_1 = center1(1);
    y_1 = center1(2);
    x_2 = center2(1);
    y_2 = center2(2);
    x_3 = center3(1);
    y_3 = center3(2);
end
%t=t+1
end
scatter(x_1,y_1,'+','linewidth',3); hold on
scatter(x_2,y_2,'+','linewidth',3); hold on
scatter(x_3,y_3,'+','linewidth',3); hold on
for i = 1:1:length(cluster1)
 distance_11 =distance_11+ sqrt((cluster1(i,1)-x_1)^2 + (cluster1(i,2)-y_1)^2);

end 
for j = 1:1:length(cluster2)
 distance_21 =distance_21+ sqrt((cluster2(j,1)-x_1)^2 + (cluster2(j,2)-y_2)^2);

end 
for z = 1:1:length(cluster3)
 distance_31 =distance_31+ sqrt((cluster3(z,1)-x_3)^2 + (cluster3(z,2)-y_3)^2);

end 
distance_c3 = distance_11+distance_21+distance_31;
%------cluster4-------------------------------

centroid_1 = [diff_x/4,diff_y/4];
centroid_2 = [2*(diff_x/4), 2*(diff_y/4)];
centroid_3 = [3*(diff_x/4), 3*(diff_y/4)];
centroid_4 = [4*(diff_x/4), 4*(diff_y/4)];

x = feature(:,1);
y = feature(:,2);
x_1 = centroid_1(1);
y_1 = centroid_1(2);
x_2 = centroid_2(1);
y_2 = centroid_2(2);
x_3 = centroid_3(1);
y_3 = centroid_3(2);
x_4 = centroid_4(1);
y_4 = centroid_4(2);
%scatter(x_1,y_1); hold on scatter(x_2,y_2); hold on scatter(x_3,y_3); hold
%on scatter(x_4,y_4);hold on
cluster1 = zeros(20,2);
cluster2 = zeros(3,2);
cluster3 = zeros(20,2);
cluster4  = zeros(15,2);

center1 = zeros(1,2);
center2 = zeros(1,2);
center3 = zeros(1,2);
center4 = zeros(1,2);
distance_1 = 0;
distance_2 = 0;
distance_3 = 0;
distance_4 = 0;
for iter = 1:1:100
    
     j = 1;
     p = 1;
     q = 1;
     r = 1;
     t =1;
    for i = 1:1:length(feature)
        dist_cen1(i) = sqrt((x_1-x(i))^2+(y_1-y(i))^2);
        dist_cen2(i) = sqrt((x_2-x(i))^2+(y_2-y(i))^2);
        dist_cen3(i) = sqrt((x_3-x(i))^2+(y_3-y(i))^2);
        dist_cen4(i) = sqrt((x_4-x(i))^2+(y_4-y(i))^2);
        if dist_cen1(i) < dist_cen2(i) & dist_cen1(i) < dist_cen3(i) & dist_cen1(i) < dist_cen4(i)
         
            cluster1(j,1) = x(i);
            cluster1(j,2) = y(i);
            j= j+1;
        elseif dist_cen2(i) < dist_cen1(i) & dist_cen2(i)< dist_cen3(i)& dist_cen2(i) < dist_cen4(i)
             cluster2(q,1) = x(i);
             cluster2(q,2) = y(i);
             q= q+1;
        elseif dist_cen3(i) < dist_cen1(i) & dist_cen3(i) < dist_cen2(i)& dist_cen3(i) < dist_cen4(i)
             cluster3(p,1) = x(i);
             cluster3(p,2) = y(i);
            p= p+1;
        elseif dist_cen4(i) < dist_cen1(i) & dist_cen4(i) < dist_cen2(i) & dist_cen4(i) < dist_cen3(i)
             cluster4(r,1) = x(i);
             cluster4(r,2) = y(i);
             r= r+1;
        end
    end

center1 = mean(cluster1);
center2 = mean(cluster2);
center3 = mean(cluster3);
center4 = mean(cluster4);
if (center1(1,1) == x_1) & (center1(1,2) ==y_1) & (center2(1,1) == x_2) & (center2(1,2)~= y_2) & (center3(1,1) == x_3) & (center3(1,2)==y_3)(center4(1,1) == x_4) & (center4(1,2)==y_4)
    break;
else
    x_1 = center1(1);
    y_1 = center1(2);
    x_2 = center2(1);
    y_2 = center2(2);
    x_3 = center3(1);
    y_3 = center3(2);
    x_4 = center4(1);
    y_4 = center4(2);
end
t=t+1
end
%scatter(r,d);
scatter(x_1,y_1,'+','linewidth',3); hold on
scatter(x_2,y_2,'+','linewidth',3); hold on
scatter(x_3,y_3,'+','linewidth',3); hold on
scatter(x_4,y_4,'+','linewidth',3); hold on
for i = 1:1:length(cluster1)
 distance_1 =distance_1+ sqrt((cluster1(i,1)-x_1)^2 + (cluster1(i,2)-y_1)^2);

end 
for j = 1:1:length(cluster2)
 distance_2 =distance_2+ sqrt((cluster2(j,1)-x_1)^2 + (cluster2(j,2)-y_2)^2);

end 
for z = 1:1:length(cluster3)
 distance_3 =distance_3+ sqrt((cluster3(z,1)-x_3)^2 + (cluster3(z,2)-y_3)^2);

end 
for x = 1:1:length(cluster4)
 distance_4 =distance_4+ sqrt((cluster4(x,1)-x_4)^2 + (cluster4(x,2)-y_4)^2);

end 
distance_c4 = distance_1+distance_2+distance_3+distance_4;

%cluster6------------------------------------------


centroid_1 = [diff_x/5,diff_y/5];
centroid_2 = [2*(diff_x/5), 2*(diff_y/5)];
centroid_3 = [3*(diff_x/5), 3*(diff_y/5)];
centroid_4 = [4*(diff_x/5), 4*(diff_y/5)];
centroid_5 = [5*(diff_x/5), 5*(diff_y/5)];
centroid_6 = [6*(diff_x/6), 6*(diff_y/6)];

x = feature(:,1);
y = feature(:,2);
x_1 = centroid_1(1);
y_1 = centroid_1(2);
x_2 = centroid_2(1);
y_2 = centroid_2(2);
x_3 = centroid_3(1);
y_3 = centroid_3(2);
x_4 = centroid_4(1);
y_4 = centroid_4(2);
x_5 = centroid_5(1);
y_5 = centroid_5(2);
%x_6 = centroid_6(1);
%y_6 = centroid_6(2);
%scatter(x_1,y_1); hold on
%scatter(x_2,y_2); hold on 
%scatter(x_3,y_3); hold on
%scatter(x_4,y_4); hold on
%scatter(x_5,y_5); hold on
%scatter(x_6,y_6); hold on
%scatter(x_1,y_1); hold on scatter(x_2,y_2); hold on scatter(x_3,y_3); hold
%on scatter(x_4,y_4);hold on
cluster1 = zeros(2,2);
cluster2 = zeros(2,2);
cluster3 = zeros(2,2);
cluster4  = zeros(2,2);
cluster5 = zeros(2,2);
%cluster6  = zeros(2,2);
center1 = zeros(1,2);
center2 = zeros(1,2);
center3 = zeros(1,2);
center4 = zeros(1,2);
center5 = zeros(1,2);
%center6 = zeros(1,2);

distance_1 = 0;
distance_2 = 0;
distance_3 = 0;
distance_4 = 0;
distance_5 = 0;
%distance_6 = 0;
for iter = 1:1:100
    
     j = 1;
     p = 1;
     q = 1;
     r = 1;
     t = 1;
     u = 1;
     %w = 1;
     matrix_f = zeros(5,1);
    for i = 1:1:length(feature)
        dist_cen1(i) = sqrt((x_1-x(i))^2+(y_1-y(i))^2);
        dist_cen2(i) = sqrt((x_2-x(i))^2+(y_2-y(i))^2);
        dist_cen3(i) = sqrt((x_3-x(i))^2+(y_3-y(i))^2);
        dist_cen4(i) = sqrt((x_4-x(i))^2+(y_4-y(i))^2);
        dist_cen5(i) = sqrt((x_5-x(i))^2+(y_5-y(i))^2);
      %  dist_cen6(i) = sqrt((x_6-x(i))^2+(y_6-y(i))^2);
        matrix_f(1,1) = dist_cen1(i);
        matrix_f(2,1) = dist_cen2(i);
        matrix_f(3,1) = dist_cen3(i);
        matrix_f(4,1) = dist_cen4(i);
        matrix_f(5,1) = dist_cen5(i);
       % matrix_f(6,1) = dist_cen6(i);
        %if dist_cen1(i) < dist_cen2(i) & dist_cen1(i) < dist_cen3(i) & dist_cen1(i) < dist_cen4(i) &dist_cen1(i) < dist_cen5(i)&dist_cen1(i) < dist_cen6(i)
         if dist_cen1(i) == min(matrix_f)
            cluster1(j,1) = x(i);
            cluster1(j,2) = y(i);
            j= j+1;
        elseif dist_cen2(i) == min(matrix_f)%dist_cen2(i) < dist_cen1(i) & dist_cen2(i)< dist_cen3(i)& dist_cen2(i) < dist_cen4(i)&dist_cen2(i) < dist_cen5(i)&dist_cen2(i) < dist_cen6(i)
             cluster2(q,1) = x(i);
             cluster2(q,2) = y(i);
             q= q+1;
        elseif dist_cen3(i) == min(matrix_f)%dist_cen3(i) < dist_cen1(i) & dist_cen3(i) < dist_cen2(i)& dist_cen3(i) < dist_cen4(i)&dist_cen3(i) < dist_cen5(i)&dist_cen3(i) < dist_cen6(i)
             cluster3(p,1) = x(i);
             cluster3(p,2) = y(i);
            p= p+1;
        elseif dist_cen4(i) == min(matrix_f)%dist_cen4(i) < dist_cen1(i) & dist_cen4(i) < dist_cen2(i) & dist_cen4(i) < dist_cen3(i)&dist_cen4(i) < dist_cen5(i)&dist_cen4(i) < dist_cen6(i)
             cluster4(r,1) = x(i);
             cluster4(r,2) = y(i);
             r= r+1;
        elseif dist_cen5(i) == min(matrix_f)%dist_cen5(i) < dist_cen1(i) & dist_cen5(i) < dist_cen2(i) & dist_cen5(i) < dist_cen3(i)&dist_cen5(i) < dist_cen4(i)&dist_cen5(i) < dist_cen6(i)
             cluster5(u,1) = x(i);
             cluster5(u,2) = y(i);
             u= u+1;
        %elseif dist_cen6(i) == min(matrix_f)%dist_cen6(i) < dist_cen1(i) & dist_cen6(i) < dist_cen2(i) & dist_cen6(i) < dist_cen3(i)&dist_cen6(i) < dist_cen4(i)&dist_cen6(i) < dist_cen5(i)
         %    cluster5(w,1) = x(i);
          %   cluster5(w,2) = y(i);
           %  w= w+1;
        end
    end

center1 = mean(cluster1);
center2 = mean(cluster2);
center3 = mean(cluster3);
center4 = mean(cluster4);
center5 = mean(cluster5);
%center6 = mean(cluster6);
if (center1(1,1) == x_1) & (center1(1,2) ==y_1) & (center2(1,1) == x_2) & (center2(1,2)~= y_2) & (center3(1,1) == x_3) & (center3(1,2)==y_3)&(center4(1,1) == x_4) & (center4(1,2)==y_4)&(center5(1,1) == x_5) & (center5(1,2)==y_5)%&(center6(1,1) == x_6) & (center6(1,2)==y_6)
    break;
else
    x_1 = center1(1);
    y_1 = center1(2);
    x_2 = center2(1);
    y_2 = center2(2);
    x_3 = center3(1);
    y_3 = center3(2);
    x_4 = center4(1);
    y_4 = center4(2);
    x_5 = center5(1);
    y_5 = center5(2);
   % x_6 = center6(1);
   % y_6 = center6(2);
end
%t=t+1
end
%scatter(r,d);
scatter(x_1,y_1,'+','linewidth',3); hold on
scatter(x_2,y_2,'+','linewidth',3); hold on
scatter(x_3,y_3,'+','linewidth',3); hold on
scatter(x_4,y_4,'+','linewidth',3); hold on
scatter(x_5,y_5,'+','linewidth',3); hold on
%scatter(x_6,y_6,'+','linewidth',3); hold on
for i = 1:1:length(cluster1)
 distance_1 =distance_1+ sqrt((cluster1(i,1)-x_1)^2 + (cluster1(i,2)-y_1)^2);

end 
for j = 1:1:length(cluster2)
 distance_2 =distance_2+ sqrt((cluster2(j,1)-x_1)^2 + (cluster2(j,2)-y_2)^2);

end 
for z = 1:1:length(cluster3)
 distance_3 =distance_3+ sqrt((cluster3(z,1)-x_3)^2 + (cluster3(z,2)-y_3)^2);

end 
for x = 1:1:length(cluster4)
 distance_4 =distance_4+ sqrt((cluster4(x,1)-x_4)^2 + (cluster4(x,2)-y_4)^2);

end 
for x = 1:1:length(cluster5)
 distance_5 =distance_5+ sqrt((cluster5(x,1)-x_5)^2 + (cluster5(x,2)-y_5)^2);

end 
%for x = 1:1:length(cluster6)
 %distance_6 =distance_6+ sqrt((cluster4(x,1)-x_6)^2 + (cluster4(x,2)-y_6)^2);

%end 
distance_c5 = distance_1+distance_2+distance_3+distance_4+distance_5;%+distance_6;
distance = zeros(3,1);
distance(1,1) = distance_c3;
distance(2,1) = distance_c4;
distance(3,1) = distance_c5;
iter = [3,4,5];
plot(iter,distance);

