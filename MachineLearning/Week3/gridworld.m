function [V] = iter_poly_eval()
gamma = 1;
sideL = 4;
nGrids = sideL^2;
V = zeros(sideL);
MAX_N_ITERS = 100; iterCnt = 0;
CONV_TOL = 1e-6; delta = 1e10;
pol_pi = 0.25;

while((delta > CONV_TOL) && (iterCnt <= MAX_N_ITERS))
    delta = 0;
    for ii=1:sideL,
        for jj=1:sideL,
            if((ii==1 && jj==1) || (ii==sideL && jj==sideL)) continue; end
                v= V(ii,jj);
                v_tmp = 0.0;

            % action = UP
            if(ii==1)
                v_tmp = v_tmp + pol_pi*(-1 + gamma*V(ii,jj));
            elseif(ii==2 && jj==1)
                v_tmp = v_tmp + pol_pi*(-1 + gamma*V(ii-1,jj));
            else
                v_tmp = v_tmp + pol_pi*(-1 + gamma*V(ii-1,jj));
            end
            
            % action = DOWN
            if( ii==sideL )
                v_tmp = v_tmp + pol_pi*(-1 + gamma*V(ii,jj));
            elseif( ii==sideL-1 && jj==sideL )
                v_tmp = v_tmp + pol_pi*(-1 + gamma*V(ii+1,jj));
            else
                v_tmp = v_tmp + pol_pi*(-1 + gamma*V(ii+1,jj));
            end

            % action = RIGHT
            if( jj==sideL )
                v_tmp = v_tmp + pol_pi*(-1 + gamma*V(ii,jj));
            elseif( jj==sideL-1 && ii==sideL )
                v_tmp = v_tmp + pol_pi*(-1 + gamma*V(ii,jj+1));
            else
                v_tmp = v_tmp + pol_pi*(-1 + gamma*V(ii,jj+1));
            end

            % action = LEFT
            if( jj==1 )
                v_tmp = v_tmp + pol_pi*(-1 + gamma*V(ii,jj));
            elseif( jj==2 && ii==1 )
                v_tmp = v_tmp + pol_pi*(-1 + gamma*V(ii,jj-1));
            else
                v_tmp = v_tmp + pol_pi*( -1 + gamma*V(ii,jj-1) );
            end

            % update V(ii,jj):
            V(ii,jj) = v_tmp;

            delta = max([delta, abs(v-V(ii,jj))]);
        end
    end

    iterCnt=iterCnt+1;
    if(0 && mod(iterCnt,1)==0)
        fprintf('iterCnt=%5d; delta=%10.5f\n', iterCnt, delta);
        disp(round(V*10)/10);
        pause
    end
end