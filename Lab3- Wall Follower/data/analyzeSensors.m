clear all;
close all;
clc;
names = {'2d120','P2Y0A21'};
n = numel(names);
subplot(1,n,1)
minDistances = [4,7];
for i = 1:n
    data = csvread(['data_',names{i},'.csv']);
    subplot(1,n,i)  
    plot(data(:,1),data(:,2))
    title(names{i});
    xlabel('distance (cm)');
    ylabel('reading (int)');
end
figure();
fits = cell(1,n);
for i = 1:n
    data = csvread(['data_',names{i},'.csv']);
    for j = 1:numel(data(:,1))
        if data(j,1)>=minDistances(i)
            break;
        end
    end
    data = data(j:end,:);
    subplot(1,n,i);
    plot(data(:,2),data(:,1))
    title(names{i});
    ylabel('distance (cm)');
    xlabel('reading (int)');
    fits{i} = polyfit(data(:,2),data(:,1),2);
    simData = fits{i}(2)*(data(:,2).^2)+fits{i}(1)*data(:,2)+fits{i}(3);
    hold on
    plot(data(:,2),simData,'r');
end
for i = 1:n
fprintf(names{i})
fprintf("\n");
disp(fits{i})
fprintf("\n");
fprintf("\n");
end

