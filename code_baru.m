%%150watt
ImaxE = 18.4;%sumbu x
ImaxD = 147.2;%sumbu y
ImaxF = 161.3;%sumbu z
ImaxA = 19;%sumbu -x
ImaxC = 148;%sumbu -y
ImaxB = 0;%sumbu -z

%%100watt
%ImaxE = 6.1;%sumbu x
%ImaxD = 80.6;%sumbu y
%ImaxF = 93.2;%sumbu z
%ImaxA = 6.3;%sumbu -x
%ImaxC = 81.8;%sumbu -y
%ImaxB = 0;%sumbu -z

%%50watt
%ImaxE = 0.1;%sumbu x
%ImaxD = 57.9;%sumbu y
%ImaxF = 61;%sumbu z
%ImaxA = 0.1;%sumbu -x
%ImaxC = 55.4;%sumbu -y
%ImaxB = 0;%sumbu -z

fr_XZ = 0:5:180;
fr_YZ = (fr_XZ*0)+90;

roll = input('Roll (rotasi terhadap sumbu x): ');
pitch = input('Pitch (rotasi terhadap sumbu y): ');
yaw = input('Yaw (rotasi terhadap sumbu z): ');

%a = 1
%b = 2
%c = 3

Raa = cosd(pitch)*cosd(yaw);
Rab = (cosd(roll)*sind(yaw)) + (sind(roll)*sind(pitch)*cosd(yaw));
Rac = (sind(pitch)*sind(yaw)) - (cosd(roll)*sind(pitch)*cosd(yaw));

Rba = -cosd(pitch)*sind(yaw);
Rbb = (cosd(roll)*cosd(yaw)) - (sind(roll)*sind(pitch)*cosd(yaw));
Rbc = (sind(roll)*cosd(yaw)) + (cosd(roll)*sind(pitch)*sind(yaw));

Rca = sind(pitch);
Rcb = -sind(roll)*cosd(pitch);
Rcc = cosd(roll)*cosd(pitch);

for i = 1:length(fr_XZ)
    arusE(i) = ImaxE*((cosd(fr_XZ(i))*Raa) + (sind(fr_XZ(i))*cosd(fr_YZ(i))*Rab) + (sind(fr_XZ(i))*sind(fr_YZ(i))*Rac));
    arusD(i) = ImaxD*((cosd(fr_XZ(i))*Rba) + (sind(fr_XZ(i))*cosd(fr_YZ(i))*Rbb) + (sind(fr_XZ(i))*sind(fr_YZ(i))*Rbc));
    arusF(i) = ImaxF*((cosd(fr_XZ(i))*Rca) + (sind(fr_XZ(i))*cosd(fr_YZ(i))*Rcb) + (sind(fr_XZ(i))*sind(fr_YZ(i))*Rcc));

    arusA(i) = (-ImaxA)*((cosd(fr_XZ(i))*Raa) + (sind(fr_XZ(i))*cosd(fr_YZ(i))*Rab) + (sind(fr_XZ(i))*sind(fr_YZ(i))*Rac));
    arusC(i) = (-ImaxC)*((cosd(fr_XZ(i))*Rba) + (sind(fr_XZ(i))*cosd(fr_YZ(i))*Rbb) + (sind(fr_XZ(i))*sind(fr_YZ(i))*Rbc));
    
    IE_init(i) = ImaxE*cosd(fr_XZ(i));
    ID_init(i) = ImaxD*sind(fr_XZ(i))*cosd(fr_YZ(i));
    IF_init(i) = ImaxF*sind(fr_XZ(i))*sind(fr_YZ(i));
    
    IA_init(i) = -ImaxA*cosd(fr_XZ(i));
    IC_init(i) = -ImaxC*sind(fr_XZ(i))*cosd(fr_YZ(i));
    
    arusE(arusE<=0) = 0;
    arusA(arusA<=0) = 0;
    arusD(arusD<=0) = 0;
    arusC(arusC<=0) = 0;
    arusF(arusF<=0) = 0;
    
    IE_init(IE_init<=0) = 0;
    IA_init(IA_init<=0) = 0;
    ID_init(ID_init<=0) = 0;
    IC_init(IC_init<=0) = 0;
    IF_init(IF_init<=0) = 0;
    
     rad_E(i) = acosd(arusE(i)/ImaxE);
     rad_D(i) = acosd(arusD(i)/ImaxD);
     rad_C(i) = acosd(arusC(i)/ImaxC);
     rad_F(i) = acosd(arusF(i)/ImaxF);
     rad_A(i) = acosd(arusA(i)/ImaxA);
     
    if arusE(i) > arusA(i)
        if arusE(i) >= IE_init(i)
            yawy(i) = 0;
            pitchy(i) = 0;
            if arusD(i) > 0 || arusC(i) > 0
                if arusD(i) > arusC(i)
                    rolly(i) = asind(arusD(i)/(ImaxD*sind(fr_XZ(i))*sind(fr_YZ(i))*cosd(yawy(i))));
                    azimuth(i) = asind(arusD(i)/(ImaxD*sind(fr_YZ(i))*cosd(yawy(i))*sind(rolly(i))));
                    az(i) = azimuth(i) - 90;
                    elev(i) = asind(-arusD(i)/(ImaxD*sind(azimuth(i))*cosd(yawy(i))));
                    opsi(i) = 1;
                elseif arusD(i) < arusC(i)
                    rolly(i) = asind(arusC(i)/(-ImaxC*sind(fr_XZ(i))*sind(fr_YZ(i))*cosd(yawy(i))));
                    azimuth(i) = asind(arusC(i)/(-ImaxC*sind(fr_YZ(i))*cosd(yawy(i))*sind(rolly(i))));
                    az(i) = azimuth(i) - 90;
                    elev(i) = asind(arusC(i)/(ImaxC*sind(azimuth(i))*cosd(yawy(i))));
                    opsi(i) = 2;
                end
            elseif arusD(i) == arusC(i)
                rolly(i) = 0;
                azimuth(i) = acosd(arusE(i)/(ImaxE*cosd(pitchy(i))*cosd(yawy(i))));
                az(i) = azimuth(i) - 90;
                elev(i) = rolly(i)*(-1);
                opsi(i) = 3;
            end
        elseif arusE(i) < IE_init(i)
            pitchy(i) = 0;
            rolly(i) = 0;
            if arusD(i) > arusC(i)
                yawy(i) = asind(arusD(i)/(-ImaxD*cosd(fr_XZ(i))*cosd(pitchy(i))));
                azimuth(i) = acosd(arusD(i)/(-ImaxD*sind(yawy(i))*cosd(pitchy(i))));
                az(i) = azimuth(i) - 90;
                elev(i) = rolly(i) * (-1);
                opsi(i) = 4;
            elseif arusD(i) < arusC(i)
                yawy(i) = asind(arusC(i)/(ImaxC*cosd(fr_XZ(i))*cosd(pitchy(i))));
                azimuth(i) = acosd(arusC(i)/(ImaxC*sind(yawy(i))*cosd(pitchy(i))));
                az(i) = azimuth(i) - 90;
                elev(i) = rolly(i) * (-1);
                opsi(i) = 5;
            elseif arusD(i) == arusC(i)
                yawy(i) = 0;
                azimuth(i) = asind(arusF(i)/(ImaxF*sind(fr_YZ(i))*cosd(rolly(i))*cosd(pitchy(i))));
                az(i) = azimuth(i) - 90;
                elev(i) = asind(arusF(i)/(ImaxF*sind(azimuth(i))*cosd(pitchy(i))));
                opsi(i) = 6;
            end
        end
        
    elseif arusE(i) < arusA(i)
        if arusA(i) >= IA_init(i)
            pitchy(i) = 0;
            yawy(i) = 0;
            if arusD(i) > 0 || arusC(i) > 0
                if arusD(i) > arusC(i)
                    rolly(i) = asind(arusD(i)/(ImaxD*sind(fr_XZ(i))*sind(fr_YZ(i))*cosd(yawy(i))));
                    azimuth(i) = 180 - asind(arusD(i)/(ImaxD*sind(fr_YZ(i))*cosd(yawy(i))*sind(rolly(i))));
                    az(i) = azimuth(i) - 90;
                    elev(i) = asind(-arusD(i)/(ImaxD*sind(azimuth(i))*cosd(yawy(i))));
                    opsi(i) = 7;
                elseif arusD(i) < arusC(i)
                    rolly(i) = asind(arusC(i)/(-ImaxC*sind(fr_XZ(i))*sind(fr_YZ(i))*cosd(yawy(i))));
                    azimuth(i) = 180 - asind(arusC(i)/(-ImaxC*sind(fr_YZ(i))*cosd(yawy(i))*sind(rolly(i))));
                    az(i) = azimuth(i) - 90;
                    elev(i) = asind(arusC(i)/(ImaxC*sind(azimuth(i))*cosd(yawy(i))));
                    opsi(i) = 8;
                end
            elseif arusD(i) == arusC(i)
                rolly(i) = 0;
                azimuth(i) = acosd(arusA(i)/(-ImaxA*cosd(pitchy(i))*cosd(yawy(i))));
                az(i) = azimuth(i) - 90;
                elev(i) = rolly(i) * (-1);
                opsi(i) = 9;
            end
        elseif arusA(i) < IA_init(i)
            pitchy(i) = 0;
            rolly(i) = 0;
            if arusD(i) > arusC(i)
                yawy(i) = asind(arusD(i)/(-ImaxD*cosd(fr_XZ(i))*cosd(pitchy(i))));
                azimuth(i) = acosd(arusD(i)/(-ImaxD*sind(yawy(i))*cosd(pitchy(i))));
                az(i) = azimuth(i) - 90;
                elev(i) = rolly(i) * (-1);
                opsi(i) = 10;
            elseif arusD(i) < arusC(i)
                yawy(i) = asind(arusC(i)/(ImaxC*cosd(fr_XZ(i))*cosd(pitchy(i))));
                azimuth(i) = acosd(arusC(i)/(ImaxC*sind(yawy(i))*cosd(pitchy(i))));
                az(i) = azimuth(i) - 90;
                elev(i) = rolly(i) * (-1);
                opsi(i) = 11;
            elseif arusD(i) == arusC(i)
                yawy(i) = 0;
                azimuth(i) = asind(arusF(i)/(ImaxF*sind(fr_YZ(i))*cosd(rolly(i))*cosd(pitchy(i))));
                az(i) = azimuth(i) - 90;
                elev(i) = asind(arusF(i)/(ImaxF*sind(azimuth(i))*cosd(pitchy(i))));
                opsi(i) = 12;
            end
        end
        
    elseif arusE(i) == arusA(i)
        if arusD(i) > 0 || arusC(i) > 0
            pitchy(i) = 0;
            if arusF(i) < IF_init(i)
                yawy(i) = 0;
                if arusD(i) > arusC(i)
                    rolly(i) = asind(arusD(i)/(ImaxD*sind(fr_XZ(i))*sind(fr_YZ(i))*cosd(yawy(i))));
                    azimuth(i) = asind(arusD(i)/(ImaxD*sind(fr_YZ(i))*cosd(yawy(i))*sind(rolly(i))));
                    az(i) = azimuth(i) - 90;
                    elev(i) = asind(-arusD(i)/(ImaxD*sind(azimuth(i))*cosd(yawy(i))));
                    opsi(i) = 13;
                elseif arusD(i) < arusC(i)
                    rolly(i) = asind(arusC(i)/(-ImaxC*sind(fr_XZ(i))*sind(fr_YZ(i))*cosd(yawy(i))));
                    azimuth(i) = asind(arusC(i)/(-ImaxC*sind(fr_YZ(i))*cosd(yawy(i))*sind(rolly(i))));
                    az(i) = azimuth(i) - 90;
                    elev(i) = asind(-arusC(i)/(-ImaxC*sind(azimuth(i))*cosd(yawy(i))));
                    opsi(i) = 14;
                end
            elseif arusF(i) >= IF_init(i)
                rolly(i) = 0;
                if arusD(i) > arusC(i)
                    yawy(i) = asind(arusD(i)/(-ImaxD*cosd(fr_XZ(i))*cosd(pitchy(i))));
                    azimuth(i) = acosd(arusD(i)/(-ImaxD*sind(yawy(i))*cosd(pitchy(i))));
                    az(i) = azimuth(i) - 90;
                    elev(i) = rolly(i) * (-1);
                    opsi(i) = 15;
                elseif arusD(i) < arusC(i)
                    yawy(i) = asind(arusC(i)/(ImaxC*cosd(fr_XZ(i))*cosd(pitchy(i))));
                    azimuth(i) = acosd(arusC(i)/(ImaxC*sind(yawy(i))*cosd(pitchy(i))));
                    az(i) = azimuth(i) - 90;
                    elev(i) = rolly(i) * (-1);
                    opsi(i) = 16;
                elseif arusD(i) == arusC(i)
                    yawy(i) = 0;
                    azimuth(i) = asind(arusF(i)/(ImaxF*sind(fr_YZ(i))*cosd(rolly(i))*cosd(pitchy(i))));
                    az(i) = azimuth(i) - 90;
                    elev(i) = asind(-arusF(i)/(ImaxF*sind(azimuth(i))*cosd(pitchy(i))));
                    opsi(i) = 17;
                end
            end
        elseif arusD(i) == arusC(i)
            pitchy(i) = 0;
            rolly(i) = 0;
            yawy(i) = 0;
            azimuth(i) = asind(arusF(i)/(ImaxF*sind(fr_YZ(i))*cosd(rolly(i))*cosd(pitchy(i))));
            az(i) = azimuth(i) - 90;
            elev(i) = asind(arusF(i)/(ImaxF*sind(azimuth(i))*cosd(pitchy(i)))) - 90;
            opsi(i) = 18;
        end
    end   
end

figure;
grafik_fr = plot(fr_XZ,fr_YZ);hold on;
grafikE = plot(az,arusE);hold on;
grafikA = plot(az,arusA);hold on;
grafikD = plot(az,arusD);hold on;
grafikC = plot(az,arusC);hold on;
grafikF = plot(az,arusF);hold on;

grafik_az = plot(az,az);hold on;
grafik_el = plot(az,elev);hold on;
grafikroll = plot(az,rolly);hold on;
grafikyaw = plot(az,yawy);hold on;
grafikpitch = plot(az,pitchy);hold on;
grafikopsi = plot(az,opsi);hold on;

grafik_radE = plot(az,rad_E);hold on;
grafik_radD = plot(az,rad_D);hold on;
grafik_radC = plot(az,rad_C);hold on;
grafik_radF = plot(az,rad_F);hold on;
grafik_radA = plot(az,rad_A);hold on;

hold off;

datasunsensor = table(grafikopsi.YData(:), grafik_fr.XData(:), grafik_fr.YData(:), grafik_az.YData(:), grafik_el.YData(:), grafikroll.YData(:), grafikpitch.YData(:), grafikyaw.YData(:), grafikE.YData(:), grafikA.YData(:), grafikD.YData(:), grafikC.YData(:), grafikF.YData(:), grafik_radE.YData(:), grafik_radD.YData(:), grafik_radC.YData(:), grafik_radF.YData(:), grafik_radA.YData(:),...
'VariableNames',{'Opsi', 'Azimuth', 'Elevasi', 'Az_rot', 'El_rot', 'Roll', 'Pitch' 'Yaw', 'arusE_', 'arusA_', 'arusD_', 'arusC_', 'arusF_', 'rad_E_', 'rad_D_', 'rad_C_', 'rad_F_', 'rad_A_'});
writetable(datasunsensor,'Yaw -45 29.xlsx');

%%

chID_skrispi1 = [1765578];
chID_skrispi2 = [1765650];
chID_skrispi3 = [1765651];
chID_skrispi4 = [1709038];

ImaxE_skrispi = [1];
ImaxC_skrispi = [2];
ImaxD_skrispi = [3];
ImaxF_skrispi = [4];
ImaxA_skrispi = [5];
El_init_skrispi = [6];
Az_init_skrispi = [7];
Status_skrispi = [8];

ArusE_skrispi = [1];
ArusC_skrispi = [2];
ArusD_skrispi = [3];
ArusF_skrispi = [4];
ArusA_skrispi = [5];
Az_rot_skrispi = [6];
El_rot_skrispi = [7];
Roll_skrispi = [8];

IncE_skrispi = [1];
IncD_skrispi = [2];
IncC_skrispi = [3];
IncF_skrispi = [4];
IncA_skrispi = [5];
Yaw_skrispi = [7];
Pitch_skrispi = [6];
SuhuE_skrispi = [8];

SuhuD_skrispi = [2];
SuhuC_skrispi = [3];
SuhuF_skrispi = [4];
SuhuA_skrispi = [5];

readAPIKey1 = 'ZVKLV02IRN6J932K';
readAPIKey2 = 'D1AVXP40EOCTKH9I';
readAPIKey3 = '3LC5Q7VJL2RBVQGG';
readAPIKey4 = 'LJ3OR3XW93PN9Q6O';


%%
%channel 1

[ImaxE_skr,time_ImaxE] = thingSpeakRead(chID_skrispi1, 'Field', ImaxE_skrispi, 'NumPoints', 1, 'ReadKey', readAPIKey1);
[ImaxD_skr,time_ImaxD] = thingSpeakRead(chID_skrispi1, 'Field', ImaxD_skrispi, 'NumPoints', 1, 'ReadKey', readAPIKey1);
[ImaxC_skr,time_ImaxC] = thingSpeakRead(chID_skrispi1, 'Field', ImaxC_skrispi, 'NumPoints', 1, 'ReadKey', readAPIKey1);
[ImaxF_skr,time_ImaxF] = thingSpeakRead(chID_skrispi1, 'Field', ImaxF_skrispi, 'NumPoints', 1, 'ReadKey', readAPIKey1);
[ImaxA_skr,time_ImaxA] = thingSpeakRead(chID_skrispi1, 'Field', ImaxA_skrispi, 'NumPoints', 1, 'ReadKey', readAPIKey1);
[El_init_skr,time_El_init] = thingSpeakRead(chID_skrispi1, 'Field', El_init_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey1);
[Az_init_skr,time_Az_init] = thingSpeakRead(chID_skrispi1, 'Field', Az_init_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey1);

%%
%channel 2

[arusE_skr,time_arusE] = thingSpeakRead(chID_skrispi2, 'Field', ArusE_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey2);
[arusD_skr,time_arusD] = thingSpeakRead(chID_skrispi2, 'Field', ArusD_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey2);
[arusC_skr,time_arusC] = thingSpeakRead(chID_skrispi2, 'Field', ArusC_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey2);
[arusF_skr,time_arusF] = thingSpeakRead(chID_skrispi2, 'Field', ArusF_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey2);
[arusA_skr,time_arusA] = thingSpeakRead(chID_skrispi2, 'Field', ArusA_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey2);
[El_rot_skr,time_El_rot] = thingSpeakRead(chID_skrispi2, 'Field', El_rot_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey2);
[Az_rot_skr,time_Az_rot] = thingSpeakRead(chID_skrispi2, 'Field', Az_rot_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey2);
[Roll_skr,time_Roll] = thingSpeakRead(chID_skrispi2, 'Field', Roll_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey2);

%%
%Channel 3

[IncE_skr,time_IncE] = thingSpeakRead(chID_skrispi3, 'Field', IncE_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey3);
[IncD_skr,time_IncD] = thingSpeakRead(chID_skrispi3, 'Field', IncD_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey3);
[IncC_skr,time_IncC] = thingSpeakRead(chID_skrispi3, 'Field', IncC_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey3);
[IncF_skr,time_IncF] = thingSpeakRead(chID_skrispi3, 'Field', IncF_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey3);
[IncA_skr,time_IncA] = thingSpeakRead(chID_skrispi3, 'Field', IncA_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey3);
[Yaw_skr,time_Yaw] = thingSpeakRead(chID_skrispi3, 'Field', Yaw_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey3);
[Pitch_skr,time_Pitch] = thingSpeakRead(chID_skrispi3, 'Field', Pitch_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey3);
[suhuE_skr,time_suhuE] = thingSpeakRead(chID_skrispi3, 'Field', SuhuE_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey3);

%%
%channel 4

[suhuD_skr,time_suhuD] = thingSpeakRead(chID_skrispi4, 'Field', SuhuD_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey4);
[suhuC_skr,time_suhuC] = thingSpeakRead(chID_skrispi4, 'Field', SuhuC_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey4);
[suhuF_skr,time_suhuF] = thingSpeakRead(chID_skrispi4, 'Field', SuhuF_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey4);
[suhuA_skr,time_suhuA] = thingSpeakRead(chID_skrispi4, 'Field', SuhuA_skrispi, 'NumPoints', 37, 'ReadKey', readAPIKey4);

%%
%generate as figures

figure;
title('Sudut penyinaran vs Azimuth');
xlabel('Azimuth (derajat)');
ylabel('Sudut penyinaran (derajat)');
hold on;
grid on;
rad_IE_skr = plot(Az_init_skr,IncE_skr,'Color',[1 0 0]);hold on;
rad_ID_skr = plot(Az_init_skr,IncD_skr,'Color',[0 1 0]);hold on;
rad_IC_skr = plot(Az_init_skr,IncC_skr,'Color',[0 0 1]);hold on;
rad_IF_skr = plot(Az_init_skr,IncF_skr,'Color',[1 0 1]);hold on;
rad_IA_skr = plot(Az_init_skr,IncA_skr,'Color',[1 1 0]);hold on;

keterangan_rad_skr = [rad_IE_skr;rad_ID_skr;rad_IC_skr;rad_IF_skr;rad_IA_skr];

legend(keterangan_rad_skr,'Incidence E','Incidence D','Incidence C','Incidence F','Incidence A');

hold off
  
figure;
title('Rotasi vs Azimuth');
xlabel('Azimuth (derajat)');
ylabel('Rotation (derajat)');
hold on;
grid on;
grafik_rolly_skr = plot(Az_init_skr,Roll_skr,'Color',[1 0 0]);hold on;
grafik_yawy_skr = plot(Az_init_skr,Yaw_skr,'Color',[0 1 0]);hold on;
grafik_pitchy_skr = plot(Az_init_skr,Pitch_skr,'Color',[0 0 1]);hold on;
keterangan_rot = [grafik_rolly_skr;grafik_yawy_skr;grafik_pitchy_skr];hold on;
legend(keterangan_rot,'Sudut roll','Sudut yaw','Sudut Pitch');
hold off;
           
figure;
title('Arus sun sensor vs Azimuth');
xlabel('Azimuth (derajat)');
ylabel('Arus sun sensor (mA)');
hold on;
grid on;
s1_skr = plot(Az_init_skr,arusE_skr,'Color',[1 0 0]);hold on;
s2_skr = plot(Az_init_skr,arusD_skr,'Color',[0 1 0]);hold on;
s3_skr = plot(Az_init_skr,arusC_skr,'Color',[0 0 1]);hold on;
s4_skr = plot(Az_init_skr,arusF_skr,'Color',[1 0 1]);hold on;
s5_skr = plot(Az_init_skr,arusA_skr,'Color',[1 1 0]);hold on;

keterangan_arus_skr =[s1_skr;s2_skr;s3_skr;s4_skr;s5_skr];

legend(keterangan_arus_skr,'Arus E','Arus D','Arus C','Arus F','Arus A');

hold off

figure;
title('Elevasi vs Azimuth');
xlabel('Sudut Azimuth (derajat)');
ylabel('Sudut elevasi Elevation (derajat)');
hold on;
grid on;
az_el_rot = plot(Az_init_skr,El_rot_skr,'Color',[0 1 0]);
hold off;

%%
%generate to excel
data_all = table(Az_init_skr, El_init_skr, Az_rot_skr, El_rot_skr, arusE_skr, arusD_skr, arusC_skr, arusF_skr, arusA_skr, IncE_skr, IncD_skr, IncC_skr, IncF_skr, IncA_skr, Roll_skr, Yaw_skr, Pitch_skr, suhuE_skr, suhuD_skr, suhuC_skr, suhuF_skr, suhuA_skr,...
'VariableNames',{'Orbit', 'Elevasi_init','Azimuth','El_rotated','ArusE','ArusD','ArusC','ArusF','ArusA','Incidence_E','Incidence_D','Incidence_C','Incidence_F','Incidence_A','Roll','Yaw','Pitch','Suhu_E','Suhu_D','Suhu_C','Suhu_F','Suhu_A'});
writetable(data_all,'Data dari thingspeak yaw -45 29 agus.xlsx');

%%

%grafik-grafik perbandingan thingspeak vs simulasi

figure;
title('Yaw sim vs Yaw real');
xlabel('Azimuth (derajat)');
ylabel('Yaw (derajat)');
hold on;
grid on;
grafik_yaw_skr = plot(az,Yaw_skr,'-*','Color',[1 0 0], 'LineWidth', 2);hold on;
grafik_yaw_skr.MarkerSize = 10;
grafik_yawy = plot(az,yawy,'Color',[0 1 0], 'LineWidth', 2);hold on;
keterangan_yaw_sim_real = [grafik_yaw_skr;grafik_yawy];hold on;
legend(keterangan_yaw_sim_real,'Sudut yaw Real','Sudut Yaw Simulasi');
hold off;

figure;
title('Roll sim vs Roll real ');
xlabel('Azimuth (derajat)');
ylabel('Roll (derajat)');
hold on;
grid on;
grafik_roll_skr = plot(az,Roll_skr,'-*','Color',[1 0 0],'LineWidth',2);hold on;
grafik_roll_skr.MarkerSize = 10;
grafik_rolly = plot(az,rolly,'Color',[0 1 0], 'LineWidth', 2);hold on;
keterangan_roll_sim_real = [grafik_roll_skr;grafik_rolly];hold on;
legend(keterangan_roll_sim_real,'Sudut roll Real','Sudut roll Simulasi');
hold off;

figure;
title('Pitch sim vs Pitch real ');
xlabel('Azimuth (derajat)');
ylabel('Pitch angle (derajat)');
hold on;
grid on;
grafik_pitch_skr = plot(az,Pitch_skr,'-*','Color',[1 0 0],'LineWidth',2);hold on;
grafik_pitch_skr.MarkerSize = 10;
grafik_pitchy = plot(az,pitchy,'Color',[0 1 0], 'LineWidth', 2);hold on;
keterangan_pitch_sim_real = [grafik_pitch_skr;grafik_pitchy];hold on;
legend(keterangan_pitch_sim_real,'Sudut pitch Real','Sudut pitch Simulasi');
hold off;

figure;
title('Azimuth sim vs Azimuth real ');
xlabel('Azimuth sim (derajat)');
ylabel('Azimuth real (derajat)');
hold on;
grid on;
grafik_az_skr = plot(az,Az_rot_skr,'-*','Color',[1 0 0],'LineWidth',2);hold on;
grafik_az_skr.MarkerSize = 10;
grafik_azz = plot(az,az,'Color',[1 1 0], 'LineWidth', 2);
ket_azz = [grafik_az_skr,grafik_azz];
legend(ket_azz,'Azimuth real','Azimuth simulasi');
hold off;
hold off;

figure;
title('Elevasi sim vs Elevasi real');
xlabel('Azimuth (derajat)');
ylabel('Elevation (derajat)');
hold on;
grid on;
az_el_skr = plot(az,El_rot_skr,'-*','Color',[1 0 0], 'LineWidth', 2);
grafik_el_skr.MarkerSize = 10;
az_el_ = plot(az,elev,'Color',[1 1 0], 'LineWidth', 2);
ket_az_el_skr = [az_el_skr,az_el_];
legend(ket_az_el_skr,'Elevasi real','Elevasi simulasi');
hold off;

figure;
title('Panel E sim vs Panel E real');
xlabel('Azimuth (derajat)');
ylabel('Arus panel E (mA)');
hold on;
grid on;
E_init_skr = plot(az,arusE_skr,'-*','Color',[1 0 0],'LineWidth',2);
E_init_skr.MarkerSize = 10;
E_rot = plot(az,arusE,'Color',[0 1 0],'LineWidth',2);

ket_E_skr =[E_init_skr;E_rot];

legend(ket_E_skr,'Arus E real','Arus E sim');

hold off;

figure;
title('Panel D sim vs Panel D real');
xlabel('Azimuth (derajat)');
ylabel('Arus panel D (mA)');
hold on;
grid on;
D_init_skr = plot(az,arusD_skr,'-*','Color',[1 0 0],'LineWidth',2);
D_init_skr.MarkerSize = 10;
D_rot = plot(az,arusD,'Color',[0 1 0],'LineWidth',2);

ket_D_skr =[D_init_skr;D_rot];

legend(ket_D_skr,'Arus D real','Arus D sim');

hold off;

figure;
title('Panel C sim vs Panel C real');
xlabel('Azimuth (derajat)');
ylabel('Arus panel C (mA)');
hold on;
grid on;
C_init_skr = plot(az,arusC_skr,'-*','Color',[1 0 0],'LineWidth',2);
C_init_skr.MarkerSize = 10;
C_rot = plot(az,arusC,'Color',[0 1 0],'LineWidth',2);

ket_C_skr =[C_init_skr;C_rot];

legend(ket_C_skr,'Arus C real','Arus C sim');

hold off;

figure;
title('Panel F sim vs Panel F real');
xlabel('Azimuth (derajat)');
ylabel('Arus panel F (mA)');
hold on;
grid on;
F_init_skr = plot(az,arusF_skr,'-*','Color',[1 0 0],'LineWidth',2);
F_init_skr.MarkerSize = 10;
F_rot = plot(az,arusF,'Color',[0 1 0],'LineWidth',2);

ket_F_skr =[F_init_skr;F_rot];

legend(ket_F_skr,'Arus F real','Arus F sim');

hold off;

figure;
title('Panel A sim vs Panel A real');
xlabel('Azimuth (derajat)');
ylabel('Arus panel A (mA)');
hold on;
grid on;
A_init_skr = plot(az,arusA_skr,'-*','Color',[1 0 0],'LineWidth',2);
A_init_skr.MarkerSize = 10;
A_rot = plot(az,arusA,'Color',[0 1 0],'LineWidth',2);

ket_A_skr =[A_init_skr;A_rot];

legend(ket_A_skr,'Arus A real','Arus A sim');

hold off;