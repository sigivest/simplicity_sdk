from pycalcmodel.core.variable import ModelVariableFormat
from pyradioconfig.parts.bobcat.calculators.calc_shaping import Calc_Shaping_Bobcat
from pyradioconfig.parts.common.utils.tinynumpy import tinynumpy
from pyradioconfig.calculator_model_framework.Utils.LogMgr import LogMgr
import numpy as np

class CalcShapingRainier(Calc_Shaping_Bobcat):

    def buildVariables(self, model):
        """Populates a list of needed variables for this calculator

        Args:
            model (ModelRoot) : Builds the variables specific to this calculator
        """
        super().buildVariables(model)

        self._addModelActual(model, 'shaping_filter_gain_iqmod', float, ModelVariableFormat.DECIMAL)

    def calc_shaping_filter_gain_actual(self, model):
        # The Ocelot shaping filter registers have changed in the register map, so need to override this function

        shaping_filter_mode = model.vars.MODEM_CTRL0_SHAPING.value
        c0 = model.vars.MODEM_SHAPING0_COEFF0.value  # SHAPING0
        c1 = model.vars.MODEM_SHAPING0_COEFF1.value
        c2 = model.vars.MODEM_SHAPING0_COEFF2.value
        c3 = model.vars.MODEM_SHAPING0_COEFF3.value
        c4 = model.vars.MODEM_SHAPING1_COEFF4.value  # SHAPING1
        c5 = model.vars.MODEM_SHAPING1_COEFF5.value
        c6 = model.vars.MODEM_SHAPING1_COEFF6.value
        c7 = model.vars.MODEM_SHAPING1_COEFF7.value
        c8 = model.vars.MODEM_SHAPING2_COEFF8.value  # SHAPING2
        c9 = model.vars.MODEM_SHAPING2_COEFF9.value
        c10 = model.vars.MODEM_SHAPING2_COEFF10.value
        c11 = model.vars.MODEM_SHAPING2_COEFF11.value
        c12 = model.vars.MODEM_SHAPING3_COEFF12.value  # SHAPING3
        c13 = model.vars.MODEM_SHAPING3_COEFF13.value
        c14 = model.vars.MODEM_SHAPING3_COEFF14.value
        c15 = model.vars.MODEM_SHAPING3_COEFF15.value
        c16 = model.vars.MODEM_SHAPING4_COEFF16.value  # SHAPING4
        c17 = model.vars.MODEM_SHAPING4_COEFF17.value
        c18 = model.vars.MODEM_SHAPING4_COEFF18.value
        c19 = model.vars.MODEM_SHAPING4_COEFF19.value
        c20 = model.vars.MODEM_SHAPING5_COEFF20.value  # SHAPING5
        c21 = model.vars.MODEM_SHAPING5_COEFF21.value
        c22 = model.vars.MODEM_SHAPING5_COEFF22.value
        c23 = model.vars.MODEM_SHAPING5_COEFF23.value
        c24 = model.vars.MODEM_SHAPING6_COEFF24.value  # SHAPING6
        c25 = model.vars.MODEM_SHAPING6_COEFF25.value
        c26 = model.vars.MODEM_SHAPING6_COEFF26.value
        c27 = model.vars.MODEM_SHAPING6_COEFF27.value
        c28 = model.vars.MODEM_SHAPING7_COEFF28.value  # SHAPING7
        c29 = model.vars.MODEM_SHAPING7_COEFF29.value
        c30 = model.vars.MODEM_SHAPING7_COEFF30.value
        c31 = model.vars.MODEM_SHAPING7_COEFF31.value
        c32 = model.vars.MODEM_SHAPING8_COEFF32.value  # SHAPING8
        c33 = model.vars.MODEM_SHAPING8_COEFF33.value
        c34 = model.vars.MODEM_SHAPING8_COEFF34.value
        c35 = model.vars.MODEM_SHAPING8_COEFF35.value
        c36 = model.vars.MODEM_SHAPING9_COEFF36.value  # SHAPING9
        c37 = model.vars.MODEM_SHAPING9_COEFF37.value
        c38 = model.vars.MODEM_SHAPING9_COEFF38.value
        c39 = model.vars.MODEM_SHAPING9_COEFF39.value
        c40 = model.vars.MODEM_SHAPING10_COEFF40.value  # SHAPING10
        c41 = model.vars.MODEM_SHAPING10_COEFF41.value
        c42 = model.vars.MODEM_SHAPING10_COEFF42.value
        c43 = model.vars.MODEM_SHAPING10_COEFF43.value
        c44 = model.vars.MODEM_SHAPING11_COEFF44.value  # SHAPING11
        c45 = model.vars.MODEM_SHAPING11_COEFF45.value
        c46 = model.vars.MODEM_SHAPING11_COEFF46.value
        c47 = model.vars.MODEM_SHAPING11_COEFF47.value
        c48 = model.vars.MODEM_SHAPING12_COEFF48.value  # SHAPING12
        c49 = model.vars.MODEM_SHAPING12_COEFF49.value
        c50 = model.vars.MODEM_SHAPING12_COEFF50.value
        c51 = model.vars.MODEM_SHAPING12_COEFF51.value
        c52 = model.vars.MODEM_SHAPING13_COEFF52.value  # SHAPING13
        c53 = model.vars.MODEM_SHAPING13_COEFF53.value
        c54 = model.vars.MODEM_SHAPING13_COEFF54.value
        c55 = model.vars.MODEM_SHAPING13_COEFF55.value
        c56 = model.vars.MODEM_SHAPING14_COEFF56.value  # SHAPING14
        c57 = model.vars.MODEM_SHAPING14_COEFF57.value
        c58 = model.vars.MODEM_SHAPING14_COEFF58.value
        c59 = model.vars.MODEM_SHAPING14_COEFF59.value
        c60 = model.vars.MODEM_SHAPING15_COEFF60.value  # SHAPING15
        c61 = model.vars.MODEM_SHAPING15_COEFF61.value
        c62 = model.vars.MODEM_SHAPING15_COEFF62.value
        c63 = model.vars.MODEM_SHAPING15_COEFF63.value

        if shaping_filter_mode == 0:
            shaping_filter_gain = 127.0 / 128.0

        elif shaping_filter_mode == 1:
            shaping_filter_gain = ((c0 + c8 + c0) + (c1 + c7) + (c2 + c6) + (c3 + c5) + (c4 + c4)) / 5.0 / 128.0

        elif shaping_filter_mode == 2:
            shaping_filter_gain = ((c0 + c7) + (c1 + c6) + (c2 + c5) + (c3 + c4)) / 4.0 / 128.0

        else:
            # use mean or max ?
            shaping_filter_gain = (c0 + c8 + c16 + c24 + c32 + c40 + c48 + c56 +
                                   c1 + c9 + c17 + c25 + c33 + c41 + c49 + c57 +
                                   c2 + c10 + c18 + c26 + c34 + c42 + c50 + c58 +
                                   c3 + c11 + c19 + c27 + c35 + c43 + c51 + c59 +
                                   c4 + c12 + c20 + c28 + c36 + c44 + c52 + c60 +
                                   c5 + c13 + c21 + c29 + c37 + c45 + c53 + c61 +
                                   c6 + c14 + c22 + c30 + c38 + c46 + c54 + c62 +
                                   c7 + c15 + c23 + c31 + c39 + c47 + c55 + c63) / 8.0 / 128.0

        model.vars.shaping_filter_gain_iqmod_actual.value = shaping_filter_gain


    def gaussian_shaping_filter(self, model, scaling_fac = 127):
        """

        Args:
            model (ModelRoot) : Data model to read and write variables from
        """

        # for gaussian pulse shapes pulse_shape_parameter holds BT value
        bt = model.vars.shaping_filter_param.value
        max_filter_taps = model.vars.max_filter_taps.value
        req_filter_taps = self.get_required_filter_taps(bt)
        modulator_select = model.vars.modulator_select.value

        if req_filter_taps > max_filter_taps:
            minumum_bt_supported = 8 / max_filter_taps
            bt = minumum_bt_supported
            LogMgr.Error("ERROR: BT < {} not supported on this part, overriding the current BT to {}".format(minumum_bt_supported, minumum_bt_supported))

        # MCUW_RADIO_CFG-2372 - IQMOD can only use EVEN Mode
        if req_filter_taps > 16 and modulator_select == model.vars.modulator_select.var_enum.IQ_MOD:
            bt = 8 / 16
            LogMgr.Error("ERROR: BT < 0.5 not supported, overriding the current BT to {}".format(bt))


        # Even mode uses 16 taps (c0, c1...c7, c7, c6... c1, c0). Therefore anything that needs less than 16 taps can be
        # implemented using Even mode
        if req_filter_taps < 17:
            shaping = 2 # sets shaping filter in Even mode
            # map BT value to standard deviation
            std = 1.05 / bt

            # generate gaussian pulse shape
            w = self.gaussian(17, std)
            # scale for unit DC gain
            w = tinynumpy.divide(w, w.sum())
            # convolve with square wave of oversampling rate width which is 8 for the shaping filter
            f_hack = tinynumpy.convolve(w, tinynumpy.ones((1, 8)).flatten())
            # scale and quantize coefficients
            c_hack = tinynumpy.round_((84.5 * f_hack))
            # keep only 8 coeffs from the peak filter tap
            coeff = c_hack[4:12]

            return coeff, shaping

        else:
            shaping = 3 # sets shaping filter in Asymmetric mode
            # map BT value to standard deviation
            std = 1.05 / bt

            # generate gaussian pulse shape
            w = self.gaussian(int(max_filter_taps + 1 - 8), std) # To get max_filter_taps points after convolution
            # scale for unit DC gain
            w = tinynumpy.divide(w, w.sum())
            # convolve with square wave of oversampling rate width which is 8 for the shaping filter
            f_hack = tinynumpy.convolve(w, tinynumpy.ones((1, 8)).flatten())
            # scale and quantize coefficients
            c_hack = tinynumpy.round_((scaling_fac * f_hack))
            # keep only non 0 coeffs as 0 coeffs in the beginning add unnecessary delay to the TX chain
            c_hack = np.array(c_hack)
            coeff = c_hack[c_hack != 0]
            # return coeffs
            return coeff, shaping
